namespace SSUToolbox.Forms
{
    partial class Mission
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
            this.generateFiles = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.missionName = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.orbiterUsed = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // generateFiles
            // 
            this.generateFiles.Location = new System.Drawing.Point(19, 100);
            this.generateFiles.Name = "generateFiles";
            this.generateFiles.Size = new System.Drawing.Size(178, 30);
            this.generateFiles.TabIndex = 0;
            this.generateFiles.Text = "Generate mission files";
            this.generateFiles.UseVisualStyleBackColor = true;
            this.generateFiles.Click += new System.EventHandler(this.generateFiles_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(16, 14);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(94, 17);
            this.label1.TabIndex = 1;
            this.label1.Text = "Mission name";
            // 
            // missionName
            // 
            this.missionName.Location = new System.Drawing.Point(116, 11);
            this.missionName.Name = "missionName";
            this.missionName.Size = new System.Drawing.Size(100, 22);
            this.missionName.TabIndex = 2;
            this.missionName.Text = "STS-X";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(16, 60);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(52, 17);
            this.label2.TabIndex = 3;
            this.label2.Text = "Orbiter";
            // 
            // orbiterUsed
            // 
            this.orbiterUsed.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.orbiterUsed.FormattingEnabled = true;
            this.orbiterUsed.Location = new System.Drawing.Point(68, 57);
            this.orbiterUsed.Name = "orbiterUsed";
            this.orbiterUsed.Size = new System.Drawing.Size(121, 24);
            this.orbiterUsed.TabIndex = 4;
            // 
            // Mission
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.orbiterUsed);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.missionName);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.generateFiles);
            this.Name = "Mission";
            this.Size = new System.Drawing.Size(324, 193);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button generateFiles;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox missionName;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox orbiterUsed;
    }
}
