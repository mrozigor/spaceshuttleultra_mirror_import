namespace SSUToolbox.Forms
{
    partial class MECO
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.mecoAlt = new System.Windows.Forms.TextBox();
            this.mecoFPA = new System.Windows.Forms.TextBox();
            this.mecoVel = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.oms1VZ = new System.Windows.Forms.TextBox();
            this.label16 = new System.Windows.Forms.Label();
            this.oms1VY = new System.Windows.Forms.TextBox();
            this.oms1VX = new System.Windows.Forms.TextBox();
            this.oms1TIG = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.oms2VZ = new System.Windows.Forms.TextBox();
            this.label17 = new System.Windows.Forms.Label();
            this.oms2VY = new System.Windows.Forms.TextBox();
            this.oms2VX = new System.Windows.Forms.TextBox();
            this.oms2TIG = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.orbiterWeight = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.targetLAN = new System.Windows.Forms.NumericUpDown();
            this.targetAlt = new System.Windows.Forms.NumericUpDown();
            this.targetInc = new System.Windows.Forms.NumericUpDown();
            this.label13 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.calculate = new System.Windows.Forms.Button();
            this.label14 = new System.Windows.Forms.Label();
            this.ascentMode = new System.Windows.Forms.ComboBox();
            this.outputScn1_06 = new System.Windows.Forms.TextBox();
            this.label15 = new System.Windows.Forms.Label();
            this.process1 = new System.Diagnostics.Process();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.targetLAN)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.targetAlt)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.targetInc)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.mecoAlt);
            this.groupBox1.Controls.Add(this.mecoFPA);
            this.groupBox1.Controls.Add(this.mecoVel);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(44, 175);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(225, 160);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "MECO";
            // 
            // mecoAlt
            // 
            this.mecoAlt.Location = new System.Drawing.Point(98, 94);
            this.mecoAlt.Name = "mecoAlt";
            this.mecoAlt.ReadOnly = true;
            this.mecoAlt.Size = new System.Drawing.Size(100, 22);
            this.mecoAlt.TabIndex = 5;
            // 
            // mecoFPA
            // 
            this.mecoFPA.Location = new System.Drawing.Point(98, 61);
            this.mecoFPA.Name = "mecoFPA";
            this.mecoFPA.ReadOnly = true;
            this.mecoFPA.Size = new System.Drawing.Size(100, 22);
            this.mecoFPA.TabIndex = 4;
            // 
            // mecoVel
            // 
            this.mecoVel.Location = new System.Drawing.Point(98, 29);
            this.mecoVel.Name = "mecoVel";
            this.mecoVel.ReadOnly = true;
            this.mecoVel.Size = new System.Drawing.Size(100, 22);
            this.mecoVel.TabIndex = 3;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(24, 94);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(68, 17);
            this.label3.TabIndex = 2;
            this.label3.Text = "MECO Alt";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(21, 61);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(78, 17);
            this.label2.TabIndex = 1;
            this.label2.Text = "MECO FPA";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(20, 32);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(72, 17);
            this.label1.TabIndex = 0;
            this.label1.Text = "MECO Vel";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.oms1VZ);
            this.groupBox2.Controls.Add(this.label16);
            this.groupBox2.Controls.Add(this.oms1VY);
            this.groupBox2.Controls.Add(this.oms1VX);
            this.groupBox2.Controls.Add(this.oms1TIG);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Location = new System.Drawing.Point(275, 175);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(225, 160);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "OMS-1 PEG 7";
            // 
            // oms1VZ
            // 
            this.oms1VZ.Location = new System.Drawing.Point(98, 130);
            this.oms1VZ.Name = "oms1VZ";
            this.oms1VZ.ReadOnly = true;
            this.oms1VZ.Size = new System.Drawing.Size(100, 22);
            this.oms1VZ.TabIndex = 7;
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(29, 130);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(26, 17);
            this.label16.TabIndex = 6;
            this.label16.Text = "VZ";
            // 
            // oms1VY
            // 
            this.oms1VY.Location = new System.Drawing.Point(98, 94);
            this.oms1VY.Name = "oms1VY";
            this.oms1VY.ReadOnly = true;
            this.oms1VY.Size = new System.Drawing.Size(100, 22);
            this.oms1VY.TabIndex = 5;
            // 
            // oms1VX
            // 
            this.oms1VX.Location = new System.Drawing.Point(98, 61);
            this.oms1VX.Name = "oms1VX";
            this.oms1VX.ReadOnly = true;
            this.oms1VX.Size = new System.Drawing.Size(100, 22);
            this.oms1VX.TabIndex = 4;
            // 
            // oms1TIG
            // 
            this.oms1TIG.Location = new System.Drawing.Point(66, 29);
            this.oms1TIG.Name = "oms1TIG";
            this.oms1TIG.ReadOnly = true;
            this.oms1TIG.Size = new System.Drawing.Size(132, 22);
            this.oms1TIG.TabIndex = 3;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(29, 94);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(26, 17);
            this.label4.TabIndex = 2;
            this.label4.Text = "VY";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(29, 61);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(26, 17);
            this.label5.TabIndex = 1;
            this.label5.Text = "VX";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(29, 32);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(31, 17);
            this.label6.TabIndex = 0;
            this.label6.Text = "TIG";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.oms2VZ);
            this.groupBox3.Controls.Add(this.label17);
            this.groupBox3.Controls.Add(this.oms2VY);
            this.groupBox3.Controls.Add(this.oms2VX);
            this.groupBox3.Controls.Add(this.oms2TIG);
            this.groupBox3.Controls.Add(this.label7);
            this.groupBox3.Controls.Add(this.label8);
            this.groupBox3.Controls.Add(this.label9);
            this.groupBox3.Location = new System.Drawing.Point(507, 175);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(225, 160);
            this.groupBox3.TabIndex = 6;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "OMS-2 PEG 7";
            // 
            // oms2VZ
            // 
            this.oms2VZ.Location = new System.Drawing.Point(98, 130);
            this.oms2VZ.Name = "oms2VZ";
            this.oms2VZ.ReadOnly = true;
            this.oms2VZ.Size = new System.Drawing.Size(100, 22);
            this.oms2VZ.TabIndex = 7;
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(29, 130);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(26, 17);
            this.label17.TabIndex = 6;
            this.label17.Text = "VZ";
            // 
            // oms2VY
            // 
            this.oms2VY.Location = new System.Drawing.Point(98, 94);
            this.oms2VY.Name = "oms2VY";
            this.oms2VY.ReadOnly = true;
            this.oms2VY.Size = new System.Drawing.Size(100, 22);
            this.oms2VY.TabIndex = 5;
            // 
            // oms2VX
            // 
            this.oms2VX.Location = new System.Drawing.Point(98, 61);
            this.oms2VX.Name = "oms2VX";
            this.oms2VX.ReadOnly = true;
            this.oms2VX.Size = new System.Drawing.Size(100, 22);
            this.oms2VX.TabIndex = 4;
            // 
            // oms2TIG
            // 
            this.oms2TIG.Location = new System.Drawing.Point(66, 29);
            this.oms2TIG.Name = "oms2TIG";
            this.oms2TIG.ReadOnly = true;
            this.oms2TIG.Size = new System.Drawing.Size(132, 22);
            this.oms2TIG.TabIndex = 3;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(29, 94);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(26, 17);
            this.label7.TabIndex = 2;
            this.label7.Text = "VY";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(29, 61);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(26, 17);
            this.label8.TabIndex = 1;
            this.label8.Text = "VX";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(29, 32);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(31, 17);
            this.label9.TabIndex = 0;
            this.label9.Text = "TIG";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.orbiterWeight);
            this.groupBox4.Controls.Add(this.label10);
            this.groupBox4.Controls.Add(this.targetLAN);
            this.groupBox4.Controls.Add(this.targetAlt);
            this.groupBox4.Controls.Add(this.targetInc);
            this.groupBox4.Controls.Add(this.label13);
            this.groupBox4.Controls.Add(this.label11);
            this.groupBox4.Controls.Add(this.label12);
            this.groupBox4.Location = new System.Drawing.Point(45, 15);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(250, 154);
            this.groupBox4.TabIndex = 7;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Input";
            // 
            // orbiterWeight
            // 
            this.orbiterWeight.Location = new System.Drawing.Point(135, 120);
            this.orbiterWeight.Name = "orbiterWeight";
            this.orbiterWeight.ReadOnly = true;
            this.orbiterWeight.Size = new System.Drawing.Size(98, 22);
            this.orbiterWeight.TabIndex = 17;
            this.orbiterWeight.Text = "0";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(23, 123);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(100, 17);
            this.label10.TabIndex = 13;
            this.label10.Text = "Orbiter Weight";
            // 
            // targetLAN
            // 
            this.targetLAN.Location = new System.Drawing.Point(135, 91);
            this.targetLAN.Name = "targetLAN";
            this.targetLAN.Size = new System.Drawing.Size(98, 22);
            this.targetLAN.TabIndex = 16;
            // 
            // targetAlt
            // 
            this.targetAlt.DecimalPlaces = 2;
            this.targetAlt.Location = new System.Drawing.Point(135, 61);
            this.targetAlt.Maximum = new decimal(new int[] {
            3000,
            0,
            0,
            0});
            this.targetAlt.Name = "targetAlt";
            this.targetAlt.Size = new System.Drawing.Size(98, 22);
            this.targetAlt.TabIndex = 14;
            this.targetAlt.Value = new decimal(new int[] {
            2500,
            0,
            0,
            65536});
            // 
            // targetInc
            // 
            this.targetInc.DecimalPlaces = 2;
            this.targetInc.Location = new System.Drawing.Point(135, 30);
            this.targetInc.Name = "targetInc";
            this.targetInc.Size = new System.Drawing.Size(98, 22);
            this.targetInc.TabIndex = 13;
            this.targetInc.Value = new decimal(new int[] {
            285,
            0,
            0,
            65536});
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(29, 93);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(81, 17);
            this.label13.TabIndex = 6;
            this.label13.Text = "Target LAN";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(29, 61);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(70, 17);
            this.label11.TabIndex = 1;
            this.label11.Text = "Target Alt";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(29, 32);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(72, 17);
            this.label12.TabIndex = 0;
            this.label12.Text = "Target Inc";
            // 
            // calculate
            // 
            this.calculate.Location = new System.Drawing.Point(567, 62);
            this.calculate.Name = "calculate";
            this.calculate.Size = new System.Drawing.Size(110, 50);
            this.calculate.TabIndex = 8;
            this.calculate.Text = "Calculate";
            this.calculate.UseVisualStyleBackColor = true;
            this.calculate.Click += new System.EventHandler(this.calculate_Click);
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(321, 79);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(43, 17);
            this.label14.TabIndex = 9;
            this.label14.Text = "Mode";
            // 
            // ascentMode
            // 
            this.ascentMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ascentMode.FormattingEnabled = true;
            this.ascentMode.Items.AddRange(new object[] {
            "Standard",
            "Direct"});
            this.ascentMode.Location = new System.Drawing.Point(379, 75);
            this.ascentMode.Name = "ascentMode";
            this.ascentMode.Size = new System.Drawing.Size(121, 24);
            this.ascentMode.TabIndex = 10;
            // 
            // outputScn1_06
            // 
            this.outputScn1_06.Location = new System.Drawing.Point(45, 381);
            this.outputScn1_06.Multiline = true;
            this.outputScn1_06.Name = "outputScn1_06";
            this.outputScn1_06.ReadOnly = true;
            this.outputScn1_06.Size = new System.Drawing.Size(677, 107);
            this.outputScn1_06.TabIndex = 11;
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(42, 361);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(138, 17);
            this.label15.TabIndex = 12;
            this.label15.Text = "SSU 1.06 scn output";
            // 
            // process1
            // 
            this.process1.StartInfo.Domain = "";
            this.process1.StartInfo.LoadUserProfile = false;
            this.process1.StartInfo.Password = null;
            this.process1.StartInfo.StandardErrorEncoding = null;
            this.process1.StartInfo.StandardOutputEncoding = null;
            this.process1.StartInfo.UserName = "";
            this.process1.SynchronizingObject = this;
            // 
            // MECO
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.label15);
            this.Controls.Add(this.outputScn1_06);
            this.Controls.Add(this.ascentMode);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.calculate);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "MECO";
            this.Size = new System.Drawing.Size(830, 502);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.targetLAN)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.targetAlt)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.targetInc)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox mecoAlt;
        private System.Windows.Forms.TextBox mecoFPA;
        private System.Windows.Forms.TextBox mecoVel;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox oms1VY;
        private System.Windows.Forms.TextBox oms1VX;
        private System.Windows.Forms.TextBox oms1TIG;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.TextBox oms2VY;
        private System.Windows.Forms.TextBox oms2VX;
        private System.Windows.Forms.TextBox oms2TIG;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Button calculate;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.ComboBox ascentMode;
        private System.Windows.Forms.TextBox outputScn1_06;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.TextBox oms1VZ;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.TextBox oms2VZ;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.NumericUpDown targetInc;
        private System.Windows.Forms.NumericUpDown targetLAN;
        private System.Windows.Forms.NumericUpDown targetAlt;
        private System.Windows.Forms.TextBox orbiterWeight;
        private System.Windows.Forms.Label label10;
        private System.Diagnostics.Process process1;
    }
}
