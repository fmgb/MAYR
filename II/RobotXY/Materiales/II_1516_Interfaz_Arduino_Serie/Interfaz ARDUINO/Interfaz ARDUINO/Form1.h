#pragma once

namespace InterfazARDUINO {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Resumen de Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: agregar código de constructor aquí
			//
		}

	protected:
		/// <summary>
		/// Limpiar los recursos que se estén utilizando.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::IO::Ports::SerialPort^  arduino;
	protected: 
	private: System::Windows::Forms::Button^  button1;

	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::CheckBox^  checkBox1;

	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Variable del diseñador requerida.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Método necesario para admitir el Diseñador. No se puede modificar
		/// el contenido del método con el editor de código.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->arduino = (gcnew System::IO::Ports::SerialPort(this->components));
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->SuspendLayout();
			// 
			// arduino
			// 
			// Cambiar por el puerto que utilice vuestro ARDUINO
			this->arduino->PortName = L"COM7";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(23, 30);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"LED";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(23, 74);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 2;
			this->button2->Text = L"Lee valor";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(138, 54);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(83, 17);
			this->checkBox1->TabIndex = 3;
			this->checkBox1->Text = L"Estado LED";
			this->checkBox1->UseVisualStyleBackColor = true;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(239, 127);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
					  this->arduino->Open();

					  this->arduino->Write( "w" );  

					  this->arduino->Close();
			 }
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
					  this->arduino->Open();

					  this->arduino->Write( "r" ); 
					  int valor = this->arduino->ReadByte();
					  if (valor == '1')
						this->checkBox1->Checked = true;
					  else
						this->checkBox1->Checked = false;

					  this->arduino->Close();
			 }
};
}

