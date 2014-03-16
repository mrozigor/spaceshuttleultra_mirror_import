using System;
//using System.Collections.Generic;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;

namespace SSUToolbox
{
    public delegate void ModalResultDelegate(DialogResult result);
    static class Program
    {
        private static Form mainForm;
        public static bool standalone = true;

        private static TextWriterTraceListener PreRun()
        {
            TextWriterTraceListener traceListener = null;
            try
            {
                FileStream traceFile = new FileStream("SSUToolbox.log", FileMode.Create);
                traceListener = new TextWriterTraceListener(traceFile);
                traceListener.TraceOutputOptions = TraceOptions.DateTime;

                Trace.Listeners.Add(traceListener);
            }
            catch (IOException)
            {
            }

            return traceListener;
        }

        [STAThread]
        static void Main()
        {
            TextWriterTraceListener traceListener = PreRun();

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(mainForm = new Form());

            PostRun(traceListener);
        }

        private static void PostRun(TextWriterTraceListener traceListener)
        {
            //Controller.exit();            //???

            if (traceListener != null)
            {
                traceListener.Flush();
                traceListener.Close();
            }
        }

        public static void Open()
        {
            CreateDialog(mainForm);
        }

        public static void Init()
        {
            standalone = false;
            mainForm = new Form();
            TextWriterTraceListener traceListener = PreRun();
            mainForm.Closed += delegate { PostRun(traceListener); };
        }

        private delegate void CreateDialogDelegate(IntPtr wrapper, IntPtr handle);

        private static CreateDialogDelegate createDialog;
        private static CreateDialogDelegate closeDialog;

        public static IntPtr CreateDialogFunction
        {
            set { createDialog = (CreateDialogDelegate)Marshal.GetDelegateForFunctionPointer(value, typeof(CreateDialogDelegate)); }
        }

        public static IntPtr CloseDialogFunction
        {
            set { closeDialog = (CreateDialogDelegate)Marshal.GetDelegateForFunctionPointer(value, typeof(CreateDialogDelegate)); }
        }

        public static void CreateDialog(Form form)
        {
            if (createDialog != null && closeDialog != null)
            {
                form.Closed += (sender, e) =>
                {
                    closeDialog(Wrapper, form.Handle);
                    form.Dispose();
                };
                CreateUnmanagedDialog(form);
            }
            else form.Show();
        }

        private static void CreateUnmanagedDialog(Form form)
        {
            form.FormBorderStyle = FormBorderStyle.None;
            mainForm.Focus();
            createDialog(Wrapper, form.Handle);
            form.Location = new Point(0, 0);
        }

        public static void CreateDialogModal(Form form, ModalResultDelegate modalFunction)
        {
            if (createDialog != null && closeDialog != null)
            {
                mainForm.Enabled = false;
                form.Closed += (sender, e) =>
                {
                    mainForm.Enabled = true;
                    closeDialog(Wrapper, form.Handle);
                    modalFunction.BeginInvoke(form.DialogResult, null, null);
                    form.Dispose();
                };
                CreateUnmanagedDialog(form);
            }
            else modalFunction(form.ShowDialog());
        }

        public static void CreateDialogModal(CommonDialog form, ModalResultDelegate modalFunction)
        {
            if (createDialog != null && closeDialog != null)
            {
                mainForm.Enabled = false;
                Thread dialog = new Thread(new ThreadStart(delegate
                {
                    var result = form.ShowDialog();
                    mainForm.Invoke(new MethodInvoker(delegate { mainForm.Enabled = true; }));
                    modalFunction.BeginInvoke(result, null, null);
                }));
                dialog.SetApartmentState(ApartmentState.STA);
                dialog.Start();
            }
            else modalFunction(form.ShowDialog());
        }

        public static IntPtr Wrapper { get; set; }
    }
}