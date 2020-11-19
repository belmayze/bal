using CommandLine;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Text.RegularExpressions;
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
            catch (Exception e)
            {
                Console.Error.WriteLine($"Error: {e.Message}");
                return 1;
            }

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

            // ルートパス
            string root_path = Path.GetDirectoryName(Path.GetFullPath(options.Input));

            foreach (XmlData.Include include in shader_container.Settings.Includes)
            {
                // パスを決定
                string path = include.Path;
                if (!Path.IsPathRooted(path))
                {
                    path = $"{root_path}\\{path}";
                }
                // ファイルのコピー
                Console.WriteLine($"Path: {path}");
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
                        string output_path = $"{working_path}\\{program.VertexShader.SourceFilePath}.vs.cso";
                        string profile_name = $"vs_{shader_container.Settings.Compile.Profile}";
                        archive_args += $" -vs {output_path}";

                        List<string> defines = new List<string>();
                        defines.Add("BAL_VERTEX_SHADER=1");

                        ConvertImpl_(process, input_path, output_path, profile_name, defines);
                    }
                    if (program.GeometryShader != null)
                    {
                        string input_path = $"{working_path}\\{program.GeometryShader.SourceFilePath}";
                        string output_path = $"{working_path}\\{program.GeometryShader.SourceFilePath}.gs.cso";
                        string profile_name = $"gs_{shader_container.Settings.Compile.Profile}";
                        archive_args += $" -gs {output_path}";

                        List<string> defines = new List<string>();
                        defines.Add("BAL_GEOMETRY_SHADER=1");

                        ConvertImpl_(process, input_path, output_path, profile_name, defines);
                    }
                    if (program.PixelShader != null)
                    {
                        string input_path = $"{working_path}\\{program.PixelShader.SourceFilePath}";
                        string output_path = $"{working_path}\\{program.PixelShader.SourceFilePath}.ps.cso";
                        string profile_name = $"ps_{shader_container.Settings.Compile.Profile}";
                        archive_args += $" -ps {output_path}";

                        List<string> defines = new List<string>();
                        defines.Add("BAL_PIXEL_SHADER=1");

                        ConvertImpl_(process, input_path, output_path, profile_name, defines);
                    }
                    if (program.ComputeShader != null)
                    {
                        string input_path = $"{working_path}\\{program.ComputeShader.SourceFilePath}";
                        string output_path = $"{working_path}\\{program.ComputeShader.SourceFilePath}.cs.cso";
                        string profile_name = $"cs_{shader_container.Settings.Compile.Profile}";
                        archive_args += $" -cs {output_path}";

                        List<string> defines = new List<string>();
                        defines.Add("BAL_COMPUTE_SHADER=1");

                        ConvertImpl_(process, input_path, output_path, profile_name, defines);
                    }
                    if (program.DomainShader != null)
                    {
                        string input_path = $"{working_path}\\{program.DomainShader.SourceFilePath}";
                        string output_path = $"{working_path}\\{program.DomainShader.SourceFilePath}.ds.cso";
                        string profile_name = $"ds_{shader_container.Settings.Compile.Profile}";
                        archive_args += $" -ds {output_path}";

                        List<string> defines = new List<string>();
                        defines.Add("BAL_DOMAIN_SHADER=1");

                        ConvertImpl_(process, input_path, output_path, profile_name, defines);
                    }
                    if (program.HullShader != null)
                    {
                        string input_path = $"{working_path}\\{program.HullShader.SourceFilePath}";
                        string output_path = $"{working_path}\\{program.HullShader.SourceFilePath}.hs.cso";
                        string profile_name = $"hs_{shader_container.Settings.Compile.Profile}";
                        archive_args += $" -hs {output_path}";

                        List<string> defines = new List<string>();
                        defines.Add("BAL_HULL_SHADER=1");

                        ConvertImpl_(process, input_path, output_path, profile_name, defines);
                    }
/*                    if (program.AmplificationShader != null)
                    {
                        string input_path = $"{working_path}\\{program.AmplificationShader.SourceFilePath}";
                        string output_path = $"{working_path}\\{program.AmplificationShader.SourceFilePath}.as.cso";
                        string profile_name = $"as_{shader_container.Settings.Compile.Profile}";
                        archive_args += $" -as {output_path}";

                        List<string> defines = new List<string>();
                        defines.Add("BAL_AMPLIFICATION_SHADER=1");

                        ConvertImpl_(process, input_path, output_path, profile_name, defines);
                    }
                    if (program.MeshShader != null)
                    {
                        string input_path = $"{working_path}\\{program.MeshShader.SourceFilePath}";
                        string output_path = $"{working_path}\\{program.MeshShader.SourceFilePath}.ms.cso";
                        string profile_name = $"ms_{shader_container.Settings.Compile.Profile}";
                        archive_args += $" -ms {output_path}";

                        List<string> defines = new List<string>();
                        defines.Add("BAL_MESH_SHADER=1");

                        ConvertImpl_(process, input_path, output_path, profile_name, defines);
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

                Console.WriteLine($"Execute: {process.StartInfo.FileName} {process.StartInfo.Arguments}");

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

        static void ConvertImpl_(Process process, string input_path, string output_path, string profile_name, List<string> defines)
        {
            process.StartInfo.Arguments = $"{input_path} /T {profile_name} /E main /Fo {output_path}";

            foreach (string define in defines)
            {
                process.StartInfo.Arguments += $" /D {define}";
            }

            Console.Out.WriteLine($"Convert Shader. [args={process.StartInfo.Arguments}]");

            process.Start();
            process.BeginOutputReadLine();
            process.BeginErrorReadLine();
            process.WaitForExit();
            process.CancelOutputRead();
            process.CancelErrorRead();
        }
    }
}
