namespace Reader
{
    partial class Form1
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.write_button = new System.Windows.Forms.Button();
            this.read_button = new System.Windows.Forms.Button();
            this.data_textBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.connect_button = new System.Windows.Forms.Button();
            this.idPort_textBox = new System.Windows.Forms.TextBox();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.statusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(34, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "ФИО";
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(47, 6);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(202, 20);
            this.textBox1.TabIndex = 1;
            // 
            // write_button
            // 
            this.write_button.Location = new System.Drawing.Point(12, 233);
            this.write_button.Name = "write_button";
            this.write_button.Size = new System.Drawing.Size(75, 23);
            this.write_button.TabIndex = 2;
            this.write_button.Text = "Запись";
            this.write_button.UseVisualStyleBackColor = true;
            this.write_button.Click += new System.EventHandler(this.write_button_Click);
            // 
            // read_button
            // 
            this.read_button.Location = new System.Drawing.Point(174, 233);
            this.read_button.Name = "read_button";
            this.read_button.Size = new System.Drawing.Size(75, 23);
            this.read_button.TabIndex = 3;
            this.read_button.Text = "Чтение";
            this.read_button.UseVisualStyleBackColor = true;
            this.read_button.Click += new System.EventHandler(this.read_button_Click);
            // 
            // data_textBox
            // 
            this.data_textBox.Location = new System.Drawing.Point(12, 74);
            this.data_textBox.Multiline = true;
            this.data_textBox.Name = "data_textBox";
            this.data_textBox.Size = new System.Drawing.Size(237, 153);
            this.data_textBox.TabIndex = 4;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 58);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(30, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Data";
            // 
            // connect_button
            // 
            this.connect_button.Location = new System.Drawing.Point(15, 32);
            this.connect_button.Name = "connect_button";
            this.connect_button.Size = new System.Drawing.Size(89, 23);
            this.connect_button.TabIndex = 6;
            this.connect_button.Text = "Подключение";
            this.connect_button.UseVisualStyleBackColor = true;
            this.connect_button.Click += new System.EventHandler(this.connect_button_Click);
            // 
            // idPort_textBox
            // 
            this.idPort_textBox.Location = new System.Drawing.Point(110, 34);
            this.idPort_textBox.Name = "idPort_textBox";
            this.idPort_textBox.Size = new System.Drawing.Size(138, 20);
            this.idPort_textBox.TabIndex = 7;
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.statusLabel});
            this.statusStrip1.Location = new System.Drawing.Point(0, 264);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(255, 22);
            this.statusStrip1.TabIndex = 8;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // statusLabel
            // 
            this.statusLabel.Name = "statusLabel";
            this.statusLabel.Size = new System.Drawing.Size(10, 17);
            this.statusLabel.Text = " ";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(255, 286);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.idPort_textBox);
            this.Controls.Add(this.connect_button);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.data_textBox);
            this.Controls.Add(this.read_button);
            this.Controls.Add(this.write_button);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.label1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button write_button;
        private System.Windows.Forms.Button read_button;
        private System.Windows.Forms.TextBox data_textBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button connect_button;
        private System.Windows.Forms.TextBox idPort_textBox;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel statusLabel;
    }
}

