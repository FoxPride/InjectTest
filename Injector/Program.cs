using System.Diagnostics;
using Reloaded.Injector;

const string InjectLibraryC = "InjectLibraryC.dll";
const string InjectLibraryStandardBootstrapper = "InjectLibraryStandard.Bootstrapper.dll";
const string InjectLibraryNETBootstrapper = "InjectLibraryNET.Bootstrapper.dll";

var process = Process.Start(new ProcessStartInfo
{
    FileName = "mspaint.exe"
});

//var process = Process.GetProcessesByName("ac_client").FirstOrDefault();

var workingLibrary = Path.GetFullPath(Path.Combine(Directory.GetCurrentDirectory(), @"..\", InjectLibraryNETBootstrapper));

var injector = new Injector(process);

var result = injector.Inject(workingLibrary);

if (result != 0)
{
    injector.Eject(workingLibrary);
}

injector.Dispose();

process?.Kill();
process?.Dispose();