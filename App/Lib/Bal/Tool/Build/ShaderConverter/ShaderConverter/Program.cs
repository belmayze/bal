using CommandLine;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Text.RegularExpressions;
using System.Xml;
using System.Xml.Serialization;

namespace ShaderConverter
{
    class Program
    {
        class Options
        {
            [Option('i', "input", Required = true, HelpText = "入力")]
            public string Input { get; set; }
            [Option('o', "output", HelpText = "出力先")]
            public string Output { get; set; }
        }

        static int Main(string[] args)
        {
            ParserResult<Options> parser_result = Parser.Default.ParseArguments<Options>(args);
            if (parser_result.Tag == ParserResultType.NotParsed)
            {
                return 1;
            }

            Options options = (parser_result as Parsed<Options>).Value;

            XmlData.ShaderContainer shader_container = null;
            try
            {
                // 入力された xml ファイルを読み込む
                FileStream stream = new FileStream(options.Input, FileMode.Open);

                try
                {
                    XmlSerializer serializer = new XmlSerializer(typeof(XmlData.ShaderContainer));
                    shader_container = (XmlData.ShaderContainer)serializer.Deserialize(stream);
                }
                catch (Exception e)
                {
                    Console.Error.WriteLine($"Error: {e.Message}");
                    return 1;
                }
            }
#if false
            catch
            {
                // ファイルがなければ作っておく（仮）
                try
                {
                    FileStream stream = new FileStream(options.Input, FileMode.CreateNew);

                    XmlSerializer serializer = new XmlSerializer(typeof(XmlData.ShaderContainer));
                    shader_container = new XmlData.ShaderContainer();

                    // 設定サンプル
                    {
                        XmlData.Include include1 = new XmlData.Include();
                        include1.Path = "..\\sample1";
                        shader_container.Settings.Includes.Add(include1);

                        XmlData.Include include2 = new XmlData.Include();
                        include2.Path = "..\\sample2";
                        shader_container.Settings.Includes.Add(include2);
                    }

                    // プログラムサンプル
                    {
                        XmlData.Program programs1 = new XmlData.Program();
                        programs1.Name = "Sample1";
                        programs1.VertexShader = new XmlData.Program.Shader();
                        programs1.VertexShader.SourceFilePath = "sample1.vs.hlsl";
                        programs1.PixelShader = new XmlData.Program.Shader();
                        programs1.PixelShader.SourceFilePath = "sample1.ps.hlsl";
                        shader_container.Programs.Add(programs1);

                        XmlData.Program programs2 = new XmlData.Program();
                        programs2.Name = "Sample2";
                        programs2.VertexShader = new XmlData.Program.Shader();
                        programs2.VertexShader.SourceFilePath = "sample2.vs.hlsl";
                        programs2.PixelShader = new XmlData.Program.Shader();
                        programs2.PixelShader.SourceFilePath = "sample2.ps.hlsl";
                        shader_container.Programs.Add(programs2);
                    }

                    serializer.Serialize(stream, shader_container);
                    shader_container = (XmlData.ShaderContainer)serializer.Deserialize(stream);
                }
                catch (Exception e)
                {
                    Console.Error.WriteLine($"Error: {e.Message}");
                    return 1;
                }
            }
#else
            catch (Exception e)
            {
                Console.Error.WriteLine($"Error: {e.Message}");
                return 1;
            }
#endif

            // include に含まれているパスをすべてtmpにコピーする
            string working_path = $"{Environment.GetEnvironmentVariable("TEMP")}\\Bal\\ShaderConverter";
            if (!Directory.Exists(working_path))
            {
                Directory.CreateDirectory(working_path);
            }
            else
            {
                string[] files = Directory.GetFiles(working_path, "*", SearchOption.AllDirectories);
                foreach (string file in files)
                {
                    File.Delete(file);
                }
            }

            foreach (XmlData.Include include in shader_container.Settings.Includes)
            {
                // パスを決定
                string path = include.Path;
                if (!Path.IsPathRooted(path))
                {
                    path = $"{Path.GetDirectoryName(options.Input)}\\{path}";
                }
                // ファイルのコピー
                string[] files = Directory.GetFiles(path, "*", SearchOption.TopDirectoryOnly);
                foreach (string file in files)
                {
                    File.Copy(file, $"{working_path}\\{Path.GetFileName(file)}");
                }
                // @TODO: 必要であればディレクトリもコピーする
            }

            // Windows10 SDK のインストール先チェック
            string windows_bin_dir;
            {
                // Windows.h が含まれているパスから SDK バージョンをチェック
                string[] windows_sdk_dir_roots = { "C:\\Program Files (x86)\\Windows Kits\\10", "C:\\Program Files\\Windows Kits\\10" };
                string windows_sdk_dir_root = "";
                string windows_sdk_version = "";
                string architexture_name = "";

                foreach (string path in windows_sdk_dir_roots)
                {
                    string[] windows_h_files = Directory.GetFiles(path, "Windows.h", SearchOption.AllDirectories);
                    foreach (string file in windows_h_files)
                    {
                        Match match = Regex.Match(file, "\\\\Include\\\\(?<version>[0-9\\.]*?)\\\\um");
                        if (match.Success && match.Groups["version"].Success)
                        {
                            windows_sdk_dir_root = path;
                            windows_sdk_version  = match.Groups["version"].Value;
                            break;
                        }
                    }
                    if (!string.IsNullOrEmpty(windows_sdk_dir_root)) { break; }
                }

                // プロセッサーのビットチェック
                string processor_architecture = Environment.GetEnvironmentVariable("PROCESSOR_ARCHITECTURE");
                if (processor_architecture.Equals("IA64") || processor_architecture.Equals("AMD64"))
                {
                    architexture_name = "x64";
                }
                else
                {
                    architexture_name = processor_architecture;
                }

                windows_bin_dir = $"{windows_sdk_dir_root}\\bin\\{windows_sdk_version}\\{architexture_name}";
            }

            // シェーダーをコンパイルして、アーカイブ用テキストファイルを作っておく
            string archive_args_filepath = $"{working_path}\\archive.args.txt";
            {
                // C++ でパッケージングする設定ファイル
                // <ProgramName> -vs <Filename> -ps <Filename>
                StreamWriter stream = new StreamWriter(archive_args_filepath, false);

                // シェーダーをコンバート
                foreach (XmlData.Program program in shader_container.Programs)
                {
                    Process process = new Process();

                    // アーカイブ用引数
                    string archive_args = program.Name;

                    // 基本設定
                    process.StartInfo.FileName = $"{windows_bin_dir}\\fxc.exe";

                    process.StartInfo.CreateNoWindow         = true;
                    process.StartInfo.UseShellExecute        = false;
                    process.StartInfo.RedirectStandardOutput = true;
                    process.StartInfo.RedirectStandardError  = true;

                    process.OutputDataReceived += StandardOutput_;
                    process.ErrorDataReceived  += StandardError_;

                    if (program.VertexShader != null)
                    {
                        string input_path = $"{working_path}\\{program.VertexShader.SourceFilePath}";
                        string output_path = $"{working_path}\\{program.VertexShader.SourceFilePath}.cso";
                        string profile_name = $"vs_{shader_container.Settings.Compile.Profile}";

                        process.StartInfo.Arguments = $"{input_path} /T {profile_name} /E main /Fo {output_path}";

                        process.Start();
                        process.BeginOutputReadLine();
                        process.BeginErrorReadLine();
                        process.WaitForExit();
                        process.CancelOutputRead();
                        process.CancelErrorRead();

                        archive_args += $" -vs {output_path}";
                    }
                    if (program.GeometryShader != null)
                    {
                        string input_path = $"{working_path}\\{program.GeometryShader.SourceFilePath}";
                        string output_path = $"{working_path}\\{program.GeometryShader.SourceFilePath}.cso";
                        string profile_name = $"gs_{shader_container.Settings.Compile.Profile}";

                        process.StartInfo.Arguments = $"{input_path} /T {profile_name} /E main /Fo {output_path}";

                        process.Start();
                        process.BeginOutputReadLine();
                        process.BeginErrorReadLine();
                        process.WaitForExit();
                        process.CancelOutputRead();
                        process.CancelErrorRead();

                        archive_args += $" -gs {output_path}";
                    }
                    if (program.PixelShader != null)
                    {
                        string input_path = $"{working_path}\\{program.PixelShader.SourceFilePath}";
                        string output_path = $"{working_path}\\{program.PixelShader.SourceFilePath}.cso";
                        string profile_name = $"ps_{shader_container.Settings.Compile.Profile}";

                        process.StartInfo.Arguments = $"{input_path} /T {profile_name} /E main /Fo {output_path}";

                        process.Start();
                        process.BeginOutputReadLine();
                        process.BeginErrorReadLine();
                        process.WaitForExit();
                        process.CancelOutputRead();
                        process.CancelErrorRead();

                        archive_args += $" -ps {output_path}";
                    }
                    if (program.ComputeShader != null)
                    {
                        string input_path = $"{working_path}\\{program.ComputeShader.SourceFilePath}";
                        string output_path = $"{working_path}\\{program.ComputeShader.SourceFilePath}.cso";
                        string profile_name = $"cs_{shader_container.Settings.Compile.Profile}";

                        process.StartInfo.Arguments = $"{input_path} /T {profile_name} /E main /Fo {output_path}";

                        process.Start();
                        process.BeginOutputReadLine();
                        process.BeginErrorReadLine();
                        process.WaitForExit();
                        process.CancelOutputRead();
                        process.CancelErrorRead();

                        archive_args += $" -cs {output_path}";
                    }
                    if (program.DomainShader != null)
                    {
                        string input_path = $"{working_path}\\{program.DomainShader.SourceFilePath}";
                        string output_path = $"{working_path}\\{program.DomainShader.SourceFilePath}.cso";
                        string profile_name = $"ds_{shader_container.Settings.Compile.Profile}";

                        process.StartInfo.Arguments = $"{input_path} /T {profile_name} /E main /Fo {output_path}";

                        process.Start();
                        process.BeginOutputReadLine();
                        process.BeginErrorReadLine();
                        process.WaitForExit();
                        process.CancelOutputRead();
                        process.CancelErrorRead();

                        archive_args += $" -ds {output_path}";
                    }
                    if (program.HullShader != null)
                    {
                        string input_path = $"{working_path}\\{program.HullShader.SourceFilePath}";
                        string output_path = $"{working_path}\\{program.HullShader.SourceFilePath}.cso";
                        string profile_name = $"hs_{shader_container.Settings.Compile.Profile}";

                        process.StartInfo.Arguments = $"{input_path} /T {profile_name} /E main /Fo {output_path}";

                        process.Start();
                        process.BeginOutputReadLine();
                        process.BeginErrorReadLine();
                        process.WaitForExit();
                        process.CancelOutputRead();
                        process.CancelErrorRead();

                        archive_args += $" -hs {output_path}";
                    }
/*                    if (program.AmplificationShader != null)
                    {
                        string input_path = $"{working_path}\\{program.AmplificationShader.SourceFilePath}";
                        string output_path = $"{working_path}\\{program.AmplificationShader.SourceFilePath}.cso";
                        string profile_name = $"as_{shader_container.Settings.Compile.Profile}";

                        process.StartInfo.Arguments = $"{input_path} /T {profile_name} /E main /Fo {output_path}";

                        process.Start();
                        process.BeginOutputReadLine();
                        process.BeginErrorReadLine();
                        process.WaitForExit();
                        process.CancelOutputRead();
                        process.CancelErrorRead();

                        archive_args += $" -as {output_path}";
                    }
                    if (program.MeshShader != null)
                    {
                        string input_path = $"{working_path}\\{program.MeshShader.SourceFilePath}";
                        string output_path = $"{working_path}\\{program.MeshShader.SourceFilePath}.cso";
                        string profile_name = $"ms_{shader_container.Settings.Compile.Profile}";

                        process.StartInfo.Arguments = $"{input_path} /T {profile_name} /E main /Fo {output_path}";

                        process.Start();
                        process.BeginOutputReadLine();
                        process.BeginErrorReadLine();
                        process.WaitForExit();
                        process.CancelOutputRead();
                        process.CancelErrorRead();

                        archive_args += $" -ms {output_path}";
                    }
*/
                    stream.WriteLine(archive_args);
                }
                stream.Close();
            }

            // アーカイブ
            {
                Process process = new Process();

                // 基本設定
                process.StartInfo.FileName = $"{Path.GetDirectoryName(Assembly.GetEntryAssembly().Location)}\\ShaderArchiver.exe";

                process.StartInfo.CreateNoWindow = true;
                process.StartInfo.UseShellExecute = false;
                process.StartInfo.RedirectStandardOutput = true;
                process.StartInfo.RedirectStandardError = true;

                process.OutputDataReceived += StandardOutput_;
                process.ErrorDataReceived += StandardError_;

                process.StartInfo.Arguments = $"-archive-list {archive_args_filepath} -output {options.Output}";

                process.Start();
                process.BeginOutputReadLine();
                process.BeginErrorReadLine();
                process.WaitForExit();
            }

            return 0;
        }

        static void StandardOutput_(object sender, DataReceivedEventArgs args)
        {
            Console.Out.WriteLine(args.Data);
        }
        static void StandardError_(object sender, DataReceivedEventArgs args)
        {
            Console.Error.WriteLine(args.Data);
        }
    }
}
