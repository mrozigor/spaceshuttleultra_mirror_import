namespace SSUToolbox.Forms
{
    partial class Kits
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.checkConfiguration = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // checkConfiguration
            // 
            this.checkConfiguration.Location = new System.Drawing.Point(17, 14);
            this.checkConfiguration.Name = "checkConfiguration";
            this.checkConfiguration.Size = new System.Drawing.Size(198, 23);
            this.checkConfiguration.TabIndex = 0;
            this.checkConfiguration.Text = "Check Configuration";
            this.checkConfiguration.UseVisualStyleBackColor = true;
            this.checkConfiguration.Click += new System.EventHandler(this.checkConfiguration_Click);
            // 
            // Kits
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.checkConfiguration);
            this.Name = "Kits";
            this.Size = new System.Drawing.Size(305, 231);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button checkConfiguration;
    }
}
