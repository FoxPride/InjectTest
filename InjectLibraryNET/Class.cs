using System.Runtime.InteropServices;

namespace InjectLibraryNET
{
    public class MyClass
    {
        [DllImport("User32.dll", CharSet = CharSet.Unicode)]
        private static extern int MessageBox(IntPtr h, string m, string c, int type);

        // This method will be called by native code inside the target process…
        [UnmanagedCallersOnly]
        public static int MyMethod()
        {
            // show modal message box
            MessageBox(IntPtr.Zero,
                ".DotNet 7.0 Managed App Injected Successfully.\n\n" +
                "Running Inside: [" + System.Diagnostics.Process.GetCurrentProcess().ProcessName + "] Process\n\n", "", 0);
            return 0;
        }
    }
}
