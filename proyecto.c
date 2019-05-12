// Incluyendo las funciones
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
// Estructuras de datos
typedef struct defDoctores{ // Estructura definida para los datos de un doctor
  char FullName[200];
  char Especialidad1[200];
  char Especialidad2[200];
  int status;// 1-> Activo y 0-> Inactivo
  char telefonoUrgencias[200];
  char Direccion[200];
  char telefono[200];
  char ConsultorioAsignado[200];
  char diasConsulta[200];
  char HorarioConsulta[50];
  char Password[200];
  struct defDoctores* sig;
}Doctores;
typedef struct defHistoria{ // Estructura definida para la historia medica de cada paciente
  char id[10];
  char NombrePaciente[200], NombreDoctor[200];
  char FechaCita[200];
  char Diagnostico[200];
  char Tratamiento[200];
  char Anotaciones[200];
  struct defHistoria* sig;
}Historia;
typedef struct defPacientes{ // Estructura definida para los datos de un paciente
  char Nombre[200];
  char Direccion[200];
  char telefono[11];
  char sexo[10];
  char edad[15];
  char fecnac[30];
  char estatura[10];
  char alergias[200];
  char tipoSangre[10];
  char PadecimientosCronicos[200];
  struct defPacientes* sig;
  Historia* HClinica;
}Pacientes;
typedef struct _defParametrosLogin{ // Estructura definida para pasar los parametros de login
  GtkWidget* entry[2], *window;
  Doctores* Lista;
  char Doctor[100];
  int bandera;
}Login;
typedef struct _defListas{ // Estructura definida para pasar como parametro las listas deinamicas
  Doctores* ListaDoctores;
  Pacientes* ListaPacientes;
  GtkWidget* entry[15];
  GtkWidget* calendar;
  GtkWidget* window;
  int sexo, flag;
  char nombreBuscado[200];
  char LoggedDoctor[200];
}ParametrosListas;
typedef struct _defGeneraHistoria{
  GtkWidget* entry[10];
  GtkWidget* window;
  Pacientes* ListaPacientes;
}GenerarHistoria;
// Prototipos de las funciones
void leerListaDoctores(Doctores**);
void leerListaPacientes(Pacientes**);
void leerHistorial(Pacientes*);
void destroy(GtkWidget* wideget, gpointer data);
GtkWidget *AddButton(GtkWidget *theBox, const gchar *buttonText, gpointer CallBackFunction, int flag);
void iniciarSesion(GtkButton *button, gpointer data);
void loger(Doctores*, Login*, int*);
void entrandoSistema(ParametrosListas*);
GtkWidget* menu(ParametrosListas* Listas);
void respuestaMenu(GtkWidget* menu, gpointer data);
void buscar();
void buscar2();
void botonesControlA(GtkButton* boton, gpointer data);
void copiarStrings(char campos[11][200]);
void copiarStrings2(char campos[11][200]);
void modificarPaciente(const gchar* nombreBuscado, const gchar* nombre, const gchar* direccion, const gchar* telefono, int sexo, const gchar* estatura, const gchar* alergias, const gchar* tipoSangre, const gchar* padecimientosCronicos, int day, int month, int year, Pacientes* ListaPacientes, GtkWidget* fecha, GtkWidget* Ed, int bandera);
void nuevoPaciente(const gchar* nombre, const gchar* direccion,const gchar* telefono, int sexo, const gchar* estatura, const gchar* alergias, const gchar* tipoSangre, const gchar* padecimientosCronicos, int day, int month, int year, Pacientes** ListaPacientes, GtkWidget* fecha, GtkWidget* Ed);
void Inserta_Inicio(Pacientes **ListaPacientes, Pacientes *nuevo);
void Inserta_Fin(Pacientes **ListaPacientes, Pacientes *nuevo);
int validarNumeros(const gchar* Cadena, char campo[]);
int validarLetras(const gchar* cadena, char campo[]);
int CalcEdad(int Dia, int Mes, int Anio);
void actualizarArchivoPacientes(Pacientes* ListaPacientes);
void radio(GtkToggleButton* button, gpointer data);
void limpiarCampos(GtkButton *button, gpointer data);
void limpiarCampos2(GtkButton *button, gpointer data);
void generarCita(ParametrosListas* datos, const gchar* nombre);
void getDate(char date[]);
void actualizarArchivoHistoria(Historia* HClinica, const gchar* paciente);
void crearCita(GtkWidget* boton, gpointer data);
void agregarNodoHistoria(Historia* Nuevo, Pacientes** temp);
void mostrarHistorial(ParametrosListas *temp,const gchar* nombre);
void DesplegarListaPacientes(GtkWidget* menu, gpointer Listas);
void interfazDoctores(GtkWidget* item, gpointer Parametros);
void botonesControlDoc(GtkWidget* button, gpointer data);
void actualizarArchivoDoctores(Doctores* ListaDoctores);
void modificarDoctor(const gchar* nombre, const gchar* direccion, const gchar* telefono, int status, const gchar* consultorio, const gchar* dias, const gchar* contra, const gchar* telefonoUrgencias, const gchar* horario, const gchar* especialidad1, const gchar* especialidad2, Doctores* ListaDoctores);
void nuevoDoctor(const gchar* nombre, const gchar* direccion, const gchar* telefono, int status, const gchar* consultorio, const gchar* dias, const gchar* contra, const gchar* telefonoUrgencias, const gchar* horario, const gchar* especialidad1, const gchar* especialidad2, Doctores** ListaDoctores);
void Inserta_Inicio2(Doctores **ListaPacientes, Doctores *nuevo);
void Inserta_Fin2(Doctores **ListaDoctores, Doctores *nuevo);
// Función principal
int main(int argc, char *argv[]) {
  Doctores* ListaDoctores = NULL;
  Pacientes* ListaPacientes = NULL;
  Login* Parametros = (Login*)malloc(sizeof(Login)); // Parametros para el inicio de sesion
  ParametrosListas* Plists = (ParametrosListas*)malloc(sizeof(ParametrosListas));
  gtk_init(&argc, &argv);
  int flag = 0; // Bandera para inciar sesión
  leerListaDoctores(&ListaDoctores); // Leyendo Doctores
  leerListaPacientes(&ListaPacientes); // Leyendo Pacientes
  leerHistorial(ListaPacientes); // Historia clinica de caada paciente
  Plists->ListaDoctores = ListaDoctores; // Asignamos las listas como parametros para el programa
  Plists->ListaPacientes = ListaPacientes;
  Plists->sexo = 0;
  loger(ListaDoctores, Parametros, &flag); // Despliega la ventana de inicio de sesion
  strcpy(Plists->LoggedDoctor, Parametros->Doctor);
  if(flag == 1){ // Si se logeo con exito, entramos al sistema
    entrandoSistema(Plists);
  }
  g_free(Parametros);
  g_free(Plists);
  return 0;
}
// Desarrollando las funciones


// Funcion para desplegar la ventana de inicio de sesion
void loger(Doctores* Lista, Login* Parametros, int* band){
  PangoAttrList *attrlist = pango_attr_list_new();
  PangoAttribute *attr = pango_attr_size_new_absolute(20 * PANGO_SCALE);
  pango_attr_list_insert(attrlist, attr);
  // Creando ventana para login
  Parametros->Lista = Lista;
  Parametros->bandera = 0;
  GtkWidget *label, *horizontal, *horizontal2, *horizontal3, *horizontal4, *label2, *vertical, *boton, *titulo;
  Parametros->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(Parametros->window), "Sistema de información médica");
  gtk_widget_set_size_request(Parametros->window, 300, 260);
  gtk_window_set_resizable(GTK_WINDOW(Parametros->window), FALSE);
  gtk_container_border_width(GTK_CONTAINER(Parametros->window), 15);
  gtk_window_set_position(GTK_WINDOW(Parametros->window), GTK_WIN_POS_CENTER_ALWAYS);
  gtk_signal_connect(GTK_OBJECT(Parametros->window), "destroy", GTK_SIGNAL_FUNC(destroy), NULL);
  // Creando las cajas
  horizontal = gtk_hbox_new(TRUE, 5);
  horizontal2 = gtk_hbox_new(TRUE, 5);
  horizontal3 = gtk_hbox_new(TRUE, 5);
  horizontal4 = gtk_hbox_new(TRUE, 5);
  vertical = gtk_vbox_new(TRUE, 5);
  titulo = gtk_label_new ("Iniciar sesión");
  gtk_misc_set_alignment (GTK_MISC(titulo), 0, 0.5);
  pango_attr_list_insert(attrlist, attr);
  gtk_label_set_attributes(GTK_LABEL(titulo), attrlist);
  //pango_attr_list_unref(attrlist); <- En el código que encontre estaba esta linea, pero dejarla sin comentar da un segmentation fault.
  gtk_box_pack_start (GTK_BOX (horizontal), titulo, TRUE, TRUE, 10);
  // Agregando un label
  label = gtk_label_new("Nombre:");
  gtk_misc_set_alignment (GTK_MISC(label), 0, 0.5);
  gtk_box_pack_start(GTK_BOX(horizontal2), label, TRUE, TRUE, 10);
  // Creando entrybox
  Parametros->entry[0] = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(horizontal2), Parametros->entry[0], TRUE, TRUE, 5);
  label2 = gtk_label_new("Contraseña:");
  gtk_misc_set_alignment (GTK_MISC(label2), 0, 0.5);
  gtk_box_pack_start(GTK_BOX(horizontal3), label2, TRUE, TRUE, 10);
  Parametros->entry[1] = gtk_entry_new();
  gtk_entry_set_visibility (GTK_ENTRY (Parametros->entry[1]), FALSE);
  gtk_entry_set_invisible_char (GTK_ENTRY (Parametros->entry[1]), '*');
  // Creando boton
  boton = AddButton(horizontal4, "Ingresar", iniciarSesion, 0);
  gtk_signal_connect(GTK_OBJECT(boton),"clicked",GTK_SIGNAL_FUNC(iniciarSesion), (gpointer)Parametros);
  gtk_box_pack_start(GTK_BOX(horizontal3), Parametros->entry[1], TRUE, TRUE, 5);
  gtk_box_pack_start(GTK_BOX(vertical), horizontal, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vertical), horizontal2, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vertical), horizontal3, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vertical), horizontal4, TRUE, TRUE, 0);
  // Muestra todo de la ventana
  gtk_container_add(GTK_CONTAINER(Parametros->window), vertical);
  gtk_widget_show_all(Parametros->window);
  gtk_main();
  *band =  Parametros->bandera;
}
// Función para leer la lista de doctores
void leerListaDoctores(Doctores** Lista){
  char Name[200];
  Doctores* Nuevo, *temp;
  FILE* Archivo = fopen("doctores.txt", "rt");
  if(Archivo == NULL){
    Archivo = fopen("doctores.txt", "wt");
    fprintf(Archivo, "DocPrueba\n");
    fprintf(Archivo, "Pediatria\n");
    fprintf(Archivo, "Ortopedia\n");
    fprintf(Archivo, "1\n");
    fprintf(Archivo, "5545455019\n");
    fprintf(Archivo, "Paseo de la Reforma 880, Lomas de Santa Fe\n");
    fprintf(Archivo, "5514129916\n");
    fprintf(Archivo, "1A\n");
    fprintf(Archivo, "Lunes y Martes\n");
    fprintf(Archivo, "07:00 - 15:00\n");
    fprintf(Archivo, "12345\n");
    fclose(Archivo);
    Archivo = fopen("doctores.txt", "rt");
  }
  while (fscanf(Archivo, " %[^\n]", Name) == 1) {
    Nuevo = (Doctores*)malloc(sizeof(Doctores));
    strcpy(Nuevo->FullName, Name);
    fscanf(Archivo, " %[^\n]", Nuevo->Especialidad1);
    fscanf(Archivo, " %[^\n]", Nuevo->Especialidad2);
    fscanf(Archivo, " %d", &Nuevo->status);
    fscanf(Archivo, " %[^\n]", Nuevo->telefonoUrgencias);
    fscanf(Archivo, " %[^\n]", Nuevo->Direccion);
    fscanf(Archivo, " %[^\n]", Nuevo->telefono);
    fscanf(Archivo, " %[^\n]", Nuevo->ConsultorioAsignado);
    fscanf(Archivo, " %[^\n]", Nuevo->diasConsulta);
    fscanf(Archivo, " %[^\n]", Nuevo->HorarioConsulta);
    fscanf(Archivo, " %[^\n]", Nuevo->Password);
    Nuevo->sig = NULL;
    if(*Lista == NULL)
      *Lista = Nuevo;
    else{
      temp = *Lista;
      while(temp->sig != NULL)
        temp = temp->sig;
      temp->sig = Nuevo;
    }
  }
  fclose(Archivo);
}
// Funcion para leer lista de pacientes
void leerListaPacientes(Pacientes** Lista){
  Pacientes* Nuevo, *temp;
  char Nombre[200];
  FILE* Archivo = fopen("pacientes.txt", "rt");
  if(Archivo == NULL)
    printf("\n");
  else{
    while (fscanf(Archivo, " %[^\n]", Nombre) == 1) {
      Nuevo = (Pacientes*)malloc(sizeof(Pacientes));
      strcpy(Nuevo->Nombre, Nombre);
      fscanf(Archivo, " %[^\n]", Nuevo->Direccion);
      fscanf(Archivo, " %[^\n]", Nuevo->telefono);
      fscanf(Archivo, " %[^\n]", Nuevo->sexo);
      fscanf(Archivo, " %[^\n]", Nuevo->fecnac);
      fscanf(Archivo, " %[^\n]", Nuevo->edad);
      fscanf(Archivo, " %[^\n]", Nuevo->estatura);
      fscanf(Archivo, " %[^\n]", Nuevo->alergias);
      fscanf(Archivo, " %[^\n]", Nuevo->tipoSangre);
      fscanf(Archivo, " %[^\n]", Nuevo->PadecimientosCronicos);
      Nuevo->sig = NULL;
      Nuevo->HClinica = NULL;
      if(*Lista == NULL)
        *Lista = Nuevo;
      else{
        temp = *Lista;
        while(temp->sig != NULL)
          temp = temp->sig;
        temp->sig = Nuevo;
      }
    }
    fclose(Archivo);
  }
}
// Funcion para leer el historial médico de cada paciente
void leerHistorial(Pacientes* Lista){
  Pacientes* temp = Lista;
  Historia* clinic, *temp2;
  int i;
  char NombreArchivo[200], id[10];
  FILE* Archivo;
  while (temp != NULL) {
    i = 0;
    strcpy(NombreArchivo, temp->Nombre);
    while (NombreArchivo[i] != '\0') {
      if(NombreArchivo[i] == ' ')
	NombreArchivo[i] = '_';
      i++;
    }
    strcat(NombreArchivo, ".txt");
    Archivo = fopen(NombreArchivo, "rt");
    if(Archivo == NULL)
      printf("\n");
    else{
      while(fscanf(Archivo, " %[^\n]", id) == 1){
	clinic = (Historia*)malloc(sizeof(Historia));
	strcpy(clinic->id, id);
	fscanf(Archivo, " %[^\n]", clinic->NombrePaciente);
	fscanf(Archivo, " %[^\n]", clinic->NombreDoctor);
	fscanf(Archivo, " %[^\n]", clinic->FechaCita);
	fscanf(Archivo, " %[^\n]", clinic->Diagnostico);
	fscanf(Archivo, " %[^\n]", clinic->Tratamiento);
	fscanf(Archivo, " %[^\n]", clinic->Anotaciones);
	clinic->sig = NULL;
	if(temp->HClinica == NULL)
	  temp->HClinica = clinic;
	else{
	  temp2 = temp->HClinica;
	  while(temp2->sig != NULL)
	    temp2 = temp2->sig;
	  temp2->sig = clinic;
	}
      }
      fclose(Archivo);
    }
    temp = temp->sig;
  }
}
// Funcion que destruye un widget
void destroy(GtkWidget* wideget, gpointer data){
  gtk_main_quit();
}
// Funcion que crea un boton
GtkWidget *AddButton(GtkWidget *theBox, const gchar *buttonText, gpointer CallBackFunction, int flag){
  GtkSettings *default_settings = gtk_settings_get_default();
  g_object_set(default_settings, "gtk-button-images", TRUE, NULL);
  GtkWidget *ingresar = gtk_image_new_from_file ("Iconos/Ingresar.png");
  gtk_widget_set_name (ingresar, "Ingresar");
  GtkWidget *buscar = gtk_image_new_from_file ("Iconos/Buscar.png");
  gtk_widget_set_name (buscar, "Buscar");
  GtkWidget *actualizar = gtk_image_new_from_file ("Iconos/Actualizar.png");
  gtk_widget_set_name (actualizar, "Actualizar");
  GtkWidget *nuevacita = gtk_image_new_from_file ("Iconos/NuevaCita.png");
  gtk_widget_set_name (nuevacita, "NuevaCita");
  GtkWidget *nuevopaciente = gtk_image_new_from_file ("Iconos/NuevoPaciente.png");
  gtk_widget_set_name (nuevopaciente, "NuevoPaciente");
  GtkWidget *historial = gtk_image_new_from_file ("Iconos/Historial.png");
  gtk_widget_set_name (historial, "Historial");
  GtkWidget *vaciar = gtk_image_new_from_file ("Iconos/Vaciar.png");
  gtk_widget_set_name (vaciar, "Vaciar");
  GtkWidget *button;

  if(flag == 0){
    button = gtk_button_new_with_label("");
    gtk_button_set_image (GTK_BUTTON (button), ingresar);
    gtk_button_set_relief (GTK_BUTTON (button), GTK_RELIEF_NONE);
    gtk_box_pack_start(GTK_BOX(theBox),button,TRUE,TRUE, 0);
  }
  if(flag == 1){
    button = gtk_button_new_with_label("");
    gtk_button_set_image (GTK_BUTTON (button), buscar);
    gtk_button_set_relief (GTK_BUTTON (button), GTK_RELIEF_NONE);
    gtk_box_pack_start(GTK_BOX(theBox),button,TRUE,TRUE, 15);
  }
  if(flag == 2){
    button = gtk_button_new_with_label("");
    gtk_button_set_image (GTK_BUTTON (button), actualizar);
    gtk_button_set_relief (GTK_BUTTON (button), GTK_RELIEF_NONE);
    gtk_box_pack_start(GTK_BOX(theBox),button,TRUE,TRUE, 15);
  }
  if(flag == 3){
    button = gtk_button_new_with_label("");
    gtk_button_set_image (GTK_BUTTON (button), nuevacita);
    gtk_button_set_relief (GTK_BUTTON (button), GTK_RELIEF_NONE);
    gtk_box_pack_start(GTK_BOX(theBox),button,TRUE,TRUE, 10);
  }
  if(flag == 4){
    button = gtk_button_new_with_label("");
    gtk_button_set_image (GTK_BUTTON (button), nuevopaciente);
    gtk_button_set_relief (GTK_BUTTON (button), GTK_RELIEF_NONE);
    gtk_box_pack_start(GTK_BOX(theBox),button,TRUE,TRUE, 10);
  }
  if(flag == 5){
    button = gtk_button_new_with_label("");
    gtk_button_set_image (GTK_BUTTON (button), historial);
    gtk_button_set_relief (GTK_BUTTON (button), GTK_RELIEF_NONE);
    gtk_box_pack_start(GTK_BOX(theBox),button,TRUE,TRUE, 10);
  }
  if(flag == 6){
    button = gtk_button_new_with_label("");
    gtk_button_set_image (GTK_BUTTON (button), vaciar);
    gtk_button_set_relief (GTK_BUTTON (button), GTK_RELIEF_NONE);
    gtk_box_pack_start(GTK_BOX(theBox),button,TRUE,TRUE, 15);
  }

  gtk_widget_show(button);
  return button;
}
// Funcion que inicia sesion haciendo la comparacion con la lista dinamica
void iniciarSesion(GtkButton *button, gpointer data){
  GtkWidget* dialog;
  Login *datos = (Login*)data;
  const gchar *nombre;
  const gchar *Password;
  char doc[100];
  Doctores* temp = datos->Lista;
  nombre = gtk_entry_get_text(GTK_ENTRY(datos->entry[0]));
  Password = gtk_entry_get_text(GTK_ENTRY(datos->entry[1]));
  while (temp != NULL && datos->bandera == 0) {
    if(strcmp(temp->FullName, nombre) == 0 && strcmp(temp->Password, Password) == 0){
      datos->bandera = 1;
      strcpy(doc, temp->FullName);
    }
    temp = temp->sig;
  }
  if(datos->bandera == 1){
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Se ha logeado con éxito");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    gtk_widget_destroy(datos->window);
    strcpy(datos->Doctor, doc);
  }
  else{
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "No ha sido encontrado en la base de datos):");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    gtk_entry_set_text(GTK_ENTRY(datos->entry[0]), "");
    gtk_entry_set_text(GTK_ENTRY(datos->entry[1]), "");
  }
}
// Funcion que muestra la ventana principal del sistema
void entrandoSistema(ParametrosListas* Listas){
  PangoAttrList *attrlist = pango_attr_list_new();
  PangoAttribute *attr = pango_attr_size_new_absolute(20 * PANGO_SCALE);
  GtkWidget* menuP, *vertical, *horizontales[11], *label[20], *invisible[11], *boton, *horizontalA, *botonesA[5], *botonLimpiar, *containerDown, *titulo, *cajatitulo;
  GtkAdjustment *ajuste;
  char campos[11][200];
  copiarStrings(campos);
  ajuste = GTK_ADJUSTMENT(gtk_adjustment_new(0.0, 0.0, 2.5, 0.2, 2.5, 0));
  // Creando las cajas
  containerDown = gtk_hbox_new(TRUE, 10);
  vertical = gtk_vbox_new(0, 0);
  for(int i = 0; i < 12; i++){
    if(i == 3)
      horizontales[i] = gtk_hbox_new(FALSE, 37);
    else
      horizontales[i] = gtk_hbox_new(TRUE, 0);
  }
  horizontalA = gtk_hbox_new(TRUE, 2);
  // Creando ventana principal
  Listas->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(Listas->window), "Sistema de información médica");
  gtk_widget_set_size_request(Listas->window, 600, 650);
  gtk_window_set_resizable(GTK_WINDOW(Listas->window), FALSE);
  gtk_window_set_position(GTK_WINDOW(Listas->window), GTK_WIN_POS_CENTER_ALWAYS);
  gtk_signal_connect(GTK_OBJECT(Listas->window), "destroy", GTK_SIGNAL_FUNC(destroy), NULL);
  menuP = menu(Listas); // Creando el menu
  gtk_box_pack_start(GTK_BOX(vertical), menuP, 0, 0, 0);

  cajatitulo = gtk_hbox_new (TRUE, 0); //Creando la caja del encabezado
  titulo = gtk_label_new ("Administración de pacientes"); //Creando el encabezado
  gtk_misc_set_alignment (GTK_MISC(titulo), 0.05, 0.5); //Alineando a la izquierda
  pango_attr_list_insert(attrlist, attr); //Llamando a la lista de atributos
  gtk_label_set_attributes(GTK_LABEL(titulo), attrlist); //Asignando atributos a título
  gtk_box_pack_start (GTK_BOX (cajatitulo), titulo, TRUE, TRUE, 0); //Título -> CajaTitulo
  gtk_box_pack_start (GTK_BOX (vertical), cajatitulo, TRUE, TRUE, 20); //CajaTitulo -> Vertical

  // Elementos principales de la interfaz
  for(int i = 0; i < 12; i++){
    if(i < 11){
      if(i == 3 || i == 4){
        if(i == 3){
          label[i] = gtk_label_new("Sexo: ");
	  gtk_box_pack_start(GTK_BOX(horizontales[i]), label[i], TRUE, TRUE, 35);
        }
        if(i == 3){
          Listas->entry[i] = gtk_radio_button_new_with_label(NULL, "M");
          gtk_box_pack_start(GTK_BOX(horizontales[3]), Listas->entry[i], FALSE, FALSE, 35);
        }else{
          Listas->entry[i] = gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(Listas->entry[3])), "F");
          gtk_box_pack_start(GTK_BOX(horizontales[3]), Listas->entry[i], FALSE, FALSE, 15);
          botonesA[i] = AddButton(horizontales[3], "Actualizar", botonesControlA, 4);
	  gtk_signal_connect(GTK_OBJECT(botonesA[i]), "clicked", GTK_SIGNAL_FUNC(botonesControlA), (gpointer)Listas);
        }
        gtk_signal_connect(GTK_OBJECT(Listas->entry[i]), "toggled", GTK_SIGNAL_FUNC(radio), (gpointer)Listas);
      }else{
        label[i] = gtk_label_new(campos[i]);
        gtk_box_pack_start(GTK_BOX(horizontales[i]), label[i], TRUE, TRUE, 0);
        if(i == 7){
          Listas->entry[i] = gtk_spin_button_new(ajuste, 0.1, 1);
        }else{
          if(i == 9){
            Listas->entry[i] = gtk_combo_box_new_text();
            gtk_combo_box_append_text(GTK_COMBO_BOX(Listas->entry[i]), "A+");
            gtk_combo_box_append_text(GTK_COMBO_BOX(Listas->entry[i]), "A-");
            gtk_combo_box_append_text(GTK_COMBO_BOX(Listas->entry[i]), "B+");
            gtk_combo_box_append_text(GTK_COMBO_BOX(Listas->entry[i]), "B-");
            gtk_combo_box_append_text(GTK_COMBO_BOX(Listas->entry[i]), "O+");
            gtk_combo_box_append_text(GTK_COMBO_BOX(Listas->entry[i]), "O-");
            gtk_combo_box_append_text(GTK_COMBO_BOX(Listas->entry[i]), "AB+");
            gtk_combo_box_set_active(GTK_COMBO_BOX(Listas->entry[i]), 0);
          }else
            Listas->entry[i] = gtk_entry_new();
        }
        gtk_box_pack_start(GTK_BOX(horizontales[i]), Listas->entry[i], TRUE, TRUE, 0);
      }

      if(i == 5   || i == 6){
        gtk_entry_set_editable(GTK_ENTRY(Listas->entry[i]), FALSE);
      }
      if(i > 4){
        invisible[i] = gtk_label_new(NULL);
        gtk_box_pack_start(GTK_BOX(horizontales[i]), invisible[i], TRUE, TRUE, 0);
      }
      if(i > 0  && i <4){
        if(i != 3){
          botonesA[i] = AddButton(horizontales[i], "Actualizar", botonesControlA, i+1);
          gtk_signal_connect(GTK_OBJECT(botonesA[i]), "clicked", GTK_SIGNAL_FUNC(botonesControlA), (gpointer)Listas);
        }
      }
      gtk_box_pack_start(GTK_BOX(vertical), horizontales[i], TRUE, TRUE, 0);
    }else{
      label[i] = gtk_label_new("Modificar/Crear fecnac: ");
      gtk_box_pack_start(GTK_BOX(horizontales[i]), label[i], TRUE, TRUE, 0);
      Listas->calendar = gtk_calendar_new();
      gtk_box_pack_start(GTK_BOX(horizontales[i]), Listas->calendar, TRUE, TRUE, 0);
      label[i] = gtk_label_new(NULL);
      gtk_box_pack_start(GTK_BOX(horizontales[i]), label[i], TRUE, TRUE, 0);
      gtk_box_pack_start(GTK_BOX(vertical), horizontales[i], TRUE, TRUE, 0);
    }
  }

  // Creando boton de busqueda
  boton = AddButton(horizontales[0], "Buscar", buscar, 1);
  gtk_signal_connect(GTK_OBJECT(boton), "clicked", GTK_SIGNAL_FUNC(buscar), (gpointer)Listas);

  botonesA[4] = AddButton(containerDown, "Actualizar", botonesControlA, 5);
  gtk_signal_connect(GTK_OBJECT(botonesA[4]), "clicked", GTK_SIGNAL_FUNC(botonesControlA), (gpointer)Listas);
  botonLimpiar = AddButton(containerDown, "Act", botonesControlA, 6);
  gtk_signal_connect(GTK_OBJECT(botonLimpiar), "clicked", GTK_SIGNAL_FUNC(limpiarCampos), (gpointer)Listas);


  invisible[10] = gtk_label_new(NULL);
  gtk_box_pack_start(GTK_BOX(vertical), horizontalA, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vertical), invisible[10], TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vertical), containerDown, TRUE, TRUE, 0);
  invisible[10] = gtk_label_new(NULL);
  gtk_box_pack_start(GTK_BOX(vertical), invisible[10], TRUE, TRUE, 0);
  gtk_container_add(GTK_CONTAINER(Listas->window), vertical);
  gtk_widget_show_all(Listas->window);
  gtk_main();
}

// Funcion para responder a las opciones del menu
void respuestaMenu(GtkWidget* menu, gpointer data){
  GtkWidget* dialog;
  if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(menu)), "Altas/Modificaciones") == 0)
    printf("Mostrará altas y modificaciones\n");
  if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(menu)), "Listado de doctores") == 0)
    printf("Listado de doctores\n");
  if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(menu)), "Desplegar doctores por especialidad") == 0)
    printf("Desplegar doctores por especialidad\n");
  if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(menu)), "Desplegar lista de pacientes") == 0)
    printf("Desplegar lista de pacientes\n");
  if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(menu)), "Ayuda general del sistema") == 0)
    printf("Ayuda del sistema\n");
  if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(menu)), "Desarrolladores") == 0){
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Desarrolladores\nCésar Mauricio Arellano Velasquez\nRaúl González Portillo\nAllan Jair Escamilla Hernández\n");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
  }
}
// Funcion que crea el menu
GtkWidget* menu(ParametrosListas* Listas){
  GtkWidget* menu, *doctoresitem, *docs, *pacientes, *pacientesitem, *ayuda, *ayudaitem, *acercade, *acercadeitem;
  // Creando menu
  docs = gtk_menu_new();
  pacientes = gtk_menu_new();
  ayuda = gtk_menu_new();
  acercade = gtk_menu_new();
  menu = gtk_menu_bar_new();
  // Creando la pestaña de doctores
  doctoresitem = gtk_menu_item_new_with_label("Doctores");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(doctoresitem), docs);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), doctoresitem);
  // Creando la pestaña de doctores
  pacientesitem = gtk_menu_item_new_with_label("Pacientes");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(pacientesitem), pacientes);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), pacientesitem);
  // Creando la pestaña de ayuda
  ayudaitem = gtk_menu_item_new_with_label("Ayuda");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(ayudaitem), ayuda);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), ayudaitem);
  // Creando la pestaña de acerca de
  acercadeitem = gtk_menu_item_new_with_label("Acerca de...");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(acercadeitem), acercade);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), acercadeitem);
  // Creando los submenus para doctores
  doctoresitem = gtk_menu_item_new_with_label("Altas/Modificaciones");
  gtk_menu_shell_append(GTK_MENU_SHELL(docs), doctoresitem);
  gtk_signal_connect(GTK_OBJECT(doctoresitem), "activate", GTK_SIGNAL_FUNC(interfazDoctores), (gpointer)Listas);
  doctoresitem = gtk_menu_item_new_with_label("Listado de doctores");
  gtk_menu_shell_append(GTK_MENU_SHELL(docs), doctoresitem);
  gtk_signal_connect(GTK_OBJECT(doctoresitem), "activate", GTK_SIGNAL_FUNC(respuestaMenu), NULL);
  doctoresitem = gtk_menu_item_new_with_label("Desplegar doctores por especialidad");
  gtk_menu_shell_append(GTK_MENU_SHELL(docs), doctoresitem);
  gtk_signal_connect(GTK_OBJECT(doctoresitem), "activate", GTK_SIGNAL_FUNC(respuestaMenu), NULL);
  // Creando los submenus para pacientes
  pacientesitem = gtk_menu_item_new_with_label("Desplegar lista de pacientes");
  gtk_menu_shell_append(GTK_MENU_SHELL(pacientes), pacientesitem);
  gtk_signal_connect(GTK_OBJECT(pacientesitem), "activate", GTK_SIGNAL_FUNC(DesplegarListaPacientes), (gpointer)Listas);
  // Creando los submenus para ayuda
  ayudaitem = gtk_menu_item_new_with_label("Ayuda general del sistema");
  gtk_menu_shell_append(GTK_MENU_SHELL(ayuda), ayudaitem);
  gtk_signal_connect(GTK_OBJECT(ayudaitem), "activate", GTK_SIGNAL_FUNC(respuestaMenu), NULL);
  // Creando los submenus para acercade
  acercadeitem = gtk_menu_item_new_with_label("Desarrolladores");
  gtk_menu_shell_append(GTK_MENU_SHELL(acercade), acercadeitem);
  gtk_signal_connect(GTK_OBJECT(acercadeitem), "activate", GTK_SIGNAL_FUNC(respuestaMenu), NULL);
  return menu;
}
// Funcion que busca a un paciente ingresado por el usuario
void buscar(GtkWidget* widget, gpointer data){
  GtkWidget *dialog;
  const gchar* nombre;
  int flag = 0, day = 0, month = 0, year = 0;
  ParametrosListas* datos = (ParametrosListas*)data;
  Pacientes* temp = datos->ListaPacientes;
  datos->flag = 0;
  nombre = gtk_entry_get_text(GTK_ENTRY(datos->entry[0]));
  if(temp == NULL){
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Aún no hay pacientes registrados");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
  }else{
    while (temp != NULL && flag == 0) {
      if(strcmp(nombre, temp->Nombre) == 0)
        flag = 1;
      else
        temp = temp->sig;
    }
    if(flag == 0){
      dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "No se ha encontrado el paciente");
      gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
      datos->nombreBuscado[0] = '\0';
      gtk_entry_set_text(GTK_ENTRY(datos->entry[1]), "");
      gtk_entry_set_text(GTK_ENTRY(datos->entry[2]), "");
      gtk_entry_set_text(GTK_ENTRY(datos->entry[5]), "");
      gtk_entry_set_text(GTK_ENTRY(datos->entry[6]), "");
      gtk_entry_set_text(GTK_ENTRY(datos->entry[7]), "");
      gtk_entry_set_text(GTK_ENTRY(datos->entry[8]), "");
      gtk_entry_set_text(GTK_ENTRY(datos->entry[10]), "");
    }else{
      dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Se ha encontrado con éxito al paciente buscado");
      gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
      gtk_entry_set_text(GTK_ENTRY(datos->entry[0]), temp->Nombre);
      strcpy(datos->nombreBuscado, temp->Nombre);
      gtk_entry_set_text(GTK_ENTRY(datos->entry[1]), temp->Direccion);
      gtk_entry_set_text(GTK_ENTRY(datos->entry[2]), temp->telefono);
      if(strcmp(temp->sexo, "Masculino") == 0){
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(datos->entry[3]), TRUE);
        datos->sexo = 0;
      }
      if(strcmp(temp->sexo, "Femenino") == 0){
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(datos->entry[4]), TRUE);
        datos->sexo = 1;
      }
      gtk_entry_set_text(GTK_ENTRY(datos->entry[5]), temp->fecnac);
      sscanf(temp->fecnac, "%d/%d/%d\n", &day, &month, &year);
      gtk_calendar_select_month(GTK_CALENDAR(datos->calendar), month-1, year);
      gtk_calendar_select_day(GTK_CALENDAR(datos->calendar), day);
      gtk_entry_set_text(GTK_ENTRY(datos->entry[6]), temp->edad);
      gtk_entry_set_text(GTK_ENTRY(datos->entry[7]), temp->estatura);
      gtk_entry_set_text(GTK_ENTRY(datos->entry[8]), temp->alergias);
      if(strcmp(temp->tipoSangre, "A+") == 0)
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[9]), 0);
      if(strcmp(temp->tipoSangre, "A-") == 0)
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[9]), 1);
      if(strcmp(temp->tipoSangre, "B+") == 0)
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[9]), 2);
      if(strcmp(temp->tipoSangre, "B-") == 0)
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[9]), 3);
      if(strcmp(temp->tipoSangre, "O+") == 0)
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[9]), 4);
      if(strcmp(temp->tipoSangre, "O-") == 0)
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[9]), 5);
      if(strcmp(temp->tipoSangre, "AB+") == 0)
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[9]), 6);
      gtk_entry_set_text(GTK_ENTRY(datos->entry[10]), temp->PadecimientosCronicos);
    }
  }
}
// Funcion que copia strings de campos
void copiarStrings(char campos[11][200]){
  strcpy(campos[0], "Nombre paciente: ");
  strcpy(campos[1], "Direccion: ");
  strcpy(campos[2], "Telefono: ");
  strcpy(campos[3], "Sexo (M/F): ");
  strcpy(campos[4], "Fecha de nacimiento: ");
  strcpy(campos[5], "Fecha de nacimiento: ");
  strcpy(campos[6], "Edad: ");
  strcpy(campos[7], "Estatura: ");
  strcpy(campos[8], "Alergias: ");
  strcpy(campos[9], "Tipo Sangre: ");
  strcpy(campos[10], "Padecimientos crónicos: ");
}
// Botones de control de la parte de abajo
void botonesControlA(GtkButton *button, gpointer data){
  ParametrosListas* datos = (ParametrosListas*)data;
  int validacion[7];
  char boton[200];
  int sexo, comboActive;
  const gchar* nombre, *direccion, *telefono, *estatura, *alergias, *tipoSangre, *padecimientosCronicos;
  guint year, month, day;
  strcpy(boton, gtk_widget_get_name(GTK_WIDGET(gtk_button_get_image(button))));
  nombre = gtk_entry_get_text(GTK_ENTRY(datos->entry[0]));
  direccion = gtk_entry_get_text(GTK_ENTRY(datos->entry[1]));
  telefono = gtk_entry_get_text(GTK_ENTRY(datos->entry[2]));
  sexo = datos->sexo;
  estatura = gtk_entry_get_text(GTK_ENTRY(datos->entry[7]));
  alergias = gtk_entry_get_text(GTK_ENTRY(datos->entry[8]));
  comboActive = gtk_combo_box_get_active(GTK_COMBO_BOX(datos->entry[9]));
  switch (comboActive) {
  case 0:
    tipoSangre = "A+";
    break;
  case 1:
    tipoSangre = "A-";
    break;
  case 2:
    tipoSangre = "B+";
    break;
  case 3:
    tipoSangre = "B-";
    break;
  case 4:
    tipoSangre = "O+";
    break;
  case 5:
    tipoSangre = "O-";
    break;
  case 6:
    tipoSangre = "AB+";
    break;
  }
  padecimientosCronicos = gtk_entry_get_text(GTK_ENTRY(datos->entry[10]));
  gtk_calendar_get_date(GTK_CALENDAR(datos->calendar), &year, &month, &day);
  validacion[0] = validarLetras(nombre, "Nombre paciente");
  validacion[1] = validarNumeros(telefono, "Telefono");
  validacion[2] = validarNumeros(estatura, "Estatura");
  validacion[3] = validarLetras(alergias, "Alergias");
  validacion[4] = validarLetras(tipoSangre, "Tipo de sangre");
  validacion[5] = validarLetras(padecimientosCronicos, "PadecimientosCronicos");
  for(int i = 0; i < 6; i++)
    if(validacion[i] == 1)
      return;
  month++;

  if(strcmp("Actualizar", boton) == 0)
    modificarPaciente(datos->nombreBuscado, nombre, direccion, telefono, sexo, estatura, alergias, tipoSangre, padecimientosCronicos, day, month, year, datos->ListaPacientes, datos->entry[5], datos->entry[6], datos->flag);
  if(strcmp("NuevoPaciente", boton) == 0){
    nuevoPaciente(nombre, direccion, telefono, sexo, estatura, alergias, tipoSangre, padecimientosCronicos, day, month, year, &datos->ListaPacientes, datos->entry[5], datos->entry[6]);
    strcpy(datos->nombreBuscado, nombre);
  }
  if(strcmp(boton, "NuevaCita") == 0){
    generarCita(datos, nombre);
    return;
  }
  if(strcmp("Historial", boton) == 0){
    mostrarHistorial(datos,nombre);
  }
}
void modificarPaciente(const gchar* nombreBuscado, const gchar* nombre, const gchar* direccion, const gchar* telefono, int sexo, const gchar* estatura, const gchar* alergias, const gchar* tipoSangre, const gchar* padecimientosCronicos, int day, int month, int year, Pacientes* ListaPacientes, GtkWidget* fecha, GtkWidget* Ed, int bandera){
  Pacientes* temp = ListaPacientes;
  GtkWidget* dialog;
  char fecnac[200], edadCaracter[10];
  int edad;
  while (temp != NULL && strcmp(nombreBuscado, temp->Nombre) != 0) {
    temp = temp->sig;
  }
  if(temp == NULL){
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "El paciente no se encuentra en la base de datos");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }
  strcpy(temp->Nombre, nombre);
  strcpy(temp->Direccion, direccion);
  strcpy(temp->telefono, telefono);
  if(sexo == 0 && bandera == 1)
    strcpy(temp->sexo, "Masculino");
  if(sexo == 1 && bandera == 1)
    strcpy(temp->sexo, "Femenino");
  strcpy(temp->estatura, estatura);
  strcpy(temp->alergias, alergias);
  strcpy(temp->tipoSangre, tipoSangre);
  strcpy(temp->PadecimientosCronicos, padecimientosCronicos);
  sprintf(fecnac, "%d/%d/%d", day, month, year);
  edad = CalcEdad(day, month, year);
  sprintf(edadCaracter, "%d", edad);
  strcpy(temp->edad, edadCaracter);
  strcpy(temp->fecnac, fecnac);

  gtk_entry_set_text(GTK_ENTRY(fecha), fecnac);
  gtk_entry_set_text(GTK_ENTRY(Ed), edadCaracter);
  actualizarArchivoPacientes(ListaPacientes);
  dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "El paciente ha sido actualizado con éxito");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}
int validarLetras(const gchar* cadena, char campo[]){
  GtkWidget* dialog;
  int i = 0;
  int flag = 0;
  if(cadena[0] == '\0'){
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "El campo %s no debe estar vacio", campo);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return 1;
  }
  while (cadena[i] != '\0' && flag == 0) {
    if(!(cadena[i] >= 'a' && cadena[i] <= 'z') && !(cadena[i] >= 'A' && cadena[i] <= 'Z') && cadena[i] != '+' && cadena[i] != '-' && cadena[i] != ' '){
      dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "El campo %s solo admite caracteres", campo);
      gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
      flag = 1;
    }
    i++;
  }
  return flag;
}
int validarNumeros(const gchar* Cadena, char campo[]){ // Funcion que valida numeros
  GtkWidget* dialog;
  int i = 0;
  int Status = 0;
  if(Cadena[0] == '\0'){
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "El campo %s no debe estar vacio", campo);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return 1;
  }
  while(Cadena[i] != '\0' && Status == 0){
    if(!(Cadena[i] >= '0' && Cadena[i] <= '9') || (i > 10)){
      if(Cadena[i] != '.'){
        dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "El campo %s solo admite maximo 10 numeros", campo);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        Status = 1;
      }
    }
    i++;
  }
  return Status;
}
int CalcEdad(int Dia, int Mes, int Anio){
  int NoDias1, NoDias2,Diferencia;
  int dd, mm, aa, Edad;
  char Timestamp[200];
  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(Timestamp,200, "%Y,%m,%d", timeinfo);
  sscanf(Timestamp,"%d,%d,%d",&aa,&mm,&dd);
  NoDias1 = (Anio*365)+(Mes*30)+Dia;
  NoDias2 = (aa*365)+(mm*30)+dd;
  Diferencia = NoDias2-NoDias1;
  Edad = Diferencia / 365;
  return Edad;
}
void actualizarArchivoPacientes(Pacientes* ListaPacientes){
  Pacientes* temp = ListaPacientes;
  FILE* Archivo = fopen("pacientes.txt", "wt");
  while(temp != NULL){
    fprintf(Archivo, "%s\n", temp->Nombre);
    fprintf(Archivo, "%s\n", temp->Direccion);
    fprintf(Archivo, "%s\n", temp->telefono);
    fprintf(Archivo, "%s\n", temp->sexo);
    fprintf(Archivo, "%s\n", temp->fecnac);
    fprintf(Archivo, "%s\n", temp->edad);
    fprintf(Archivo, "%s\n", temp->estatura);
    fprintf(Archivo, "%s\n", temp->alergias);
    fprintf(Archivo, "%s\n", temp->tipoSangre);
    fprintf(Archivo, "%s\n", temp->PadecimientosCronicos);
    temp = temp->sig;
  }
  fclose(Archivo);
}
void radio(GtkToggleButton* button, gpointer data){
  ParametrosListas* datos = (ParametrosListas*)data;
  if(gtk_toggle_button_get_active(button)){
    if(datos->sexo == 0)
      datos->sexo = 1;
    else
      datos->sexo = 0;
    datos->flag = 1;
  }
}

void limpiarCampos(GtkButton *button, gpointer data){
  ParametrosListas* datos = (ParametrosListas*)data;
  datos->nombreBuscado[0] = '\0';
  for(int i = 0; i < 11; i++)
    if(i != 4 && i != 3 && i != 9)
      gtk_entry_set_text(GTK_ENTRY(datos->entry[i]), "");
  gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[9]), 0);
}

void nuevoPaciente(const gchar* nombre, const gchar* direccion,const gchar* telefono, int sexo, const gchar* estatura, const gchar* alergias, const gchar* tipoSangre, const gchar* padecimientosCronicos, int day, int month, int year, Pacientes** ListaPacientes, GtkWidget* fecha, GtkWidget* Ed){
  Pacientes* temp2 = *ListaPacientes;
  Pacientes* temp3 = *ListaPacientes;
  Pacientes* temp4 = *ListaPacientes;
  Pacientes* temp = (Pacientes*)malloc(sizeof(Pacientes));
  GtkWidget* dialog;
  int Bandera = 0;
  while (temp3 != NULL && Bandera == 0) {
    if(strcmp(temp3->Nombre, nombre) == 0)
      Bandera = 1;
    temp3 = temp3->sig;
  }
  if(Bandera == 1){
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "No se puede agregar este paciente debido a que ya existe uno con el mismo nombre");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }
  char fecnac[200], edadCaracter[10];
  int edad;
  strcpy(temp->Nombre, nombre);
  strcpy(temp->Direccion, direccion);
  strcpy(temp->telefono, telefono);
  if(sexo == 0)
    strcpy(temp->sexo, "Masculino");
  else
    strcpy(temp->sexo, "Femenino");
  strcpy(temp->estatura, estatura);
  strcpy(temp->alergias, alergias);
  strcpy(temp->tipoSangre, tipoSangre);
  strcpy(temp->PadecimientosCronicos, padecimientosCronicos);
  sprintf(fecnac, "%d/%d/%d", day, month, year);
  edad = CalcEdad(day, month, year);
  sprintf(edadCaracter, "%d", edad);
  strcpy(temp->edad, edadCaracter);
  strcpy(temp->fecnac, fecnac);
  temp->HClinica = NULL;
  temp->sig = NULL;
  while((temp2!=NULL)&&(strcmp(nombre,temp2->Nombre)>0))
    temp2=temp2->sig;

  if(temp2!=NULL)
    {
      if(temp2!=*ListaPacientes)
	{
	  while(temp4->sig!=temp2)
	    temp4=temp4->sig;
	  temp->sig=temp2;
	  temp4->sig=temp;
	}
      else
	Inserta_Inicio(ListaPacientes,temp);
    }
  else
    Inserta_Fin(ListaPacientes,temp);

  actualizarArchivoPacientes(*ListaPacientes);
  dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Se ha agregado de manera correcta el paciente");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
  gtk_entry_set_text(GTK_ENTRY(fecha), fecnac);
  gtk_entry_set_text(GTK_ENTRY(Ed), edadCaracter);
}
void Inserta_Inicio(Pacientes **ListaPacientes, Pacientes *nuevo){
  nuevo->sig=*ListaPacientes;
  *ListaPacientes=nuevo;
}

void Inserta_Fin(Pacientes **ListaPacientes, Pacientes *nuevo){
  Pacientes *temp2;
  nuevo->sig=NULL;
  if(*ListaPacientes!=NULL)
    {
      temp2=*ListaPacientes;
      while(temp2->sig!=NULL)
	temp2=temp2->sig;
      temp2->sig=nuevo;
    }
  else
    *ListaPacientes=nuevo;
}
void generarCita(ParametrosListas* datos, const gchar* nombre){
  GtkSettings *default_settings = gtk_settings_get_default();
  g_object_set(default_settings, "gtk-button-images", TRUE, NULL);
  GtkWidget *menuP, *vertical, *label[10], *horizontales[10], *invisible[3], *boton, *titulo;
  GenerarHistoria* Entradas = (GenerarHistoria*)malloc(sizeof(GenerarHistoria));
  Entradas->ListaPacientes = datos->ListaPacientes;
  char date[100];
  getDate(date);
  titulo = gtk_label_new("Generar Cita");
  vertical = gtk_vbox_new(FALSE, 0);
  for(int i = 0; i < 10; i++)
    horizontales[i] = gtk_hbox_new(TRUE, 0);
  Entradas->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(Entradas->window), "Sistema de información médica");
  gtk_widget_set_size_request(Entradas->window, 450, 500);
  gtk_window_set_resizable(GTK_WINDOW(Entradas->window), FALSE);
  gtk_window_set_position(GTK_WINDOW(Entradas->window), GTK_WIN_POS_CENTER_ALWAYS);
  gtk_signal_connect(GTK_OBJECT(Entradas->window), "destroy", GTK_SIGNAL_FUNC(destroy), NULL);
  menuP = menu(datos); // Creando el menu
  gtk_box_pack_start(GTK_BOX(vertical), menuP, 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(vertical), titulo, 0, 0, 0);
  label[0] = gtk_label_new("Paciente: ");
  label[1] = gtk_label_new("Doctor Tratante: ");
  label[2] = gtk_label_new("Fecha de cita: ");
  label[3] = gtk_label_new("Diagnostico: ");
  label[4] = gtk_label_new("Tratamiento: ");
  label[5] = gtk_label_new("Anotaciones: ");
  for(int i = 0; i < 6; i++){
    Entradas->entry[i] = gtk_entry_new();
    if(i == 0 || i == 1 || i == 2)
      gtk_entry_set_editable(GTK_ENTRY(Entradas->entry[i]), FALSE);
    if(i == 0)
      gtk_entry_set_text(GTK_ENTRY(Entradas->entry[i]), nombre);
    if(i == 1)
      gtk_entry_set_text(GTK_ENTRY(Entradas->entry[i]), datos->LoggedDoctor);
    if(i == 2)
      gtk_entry_set_text(GTK_ENTRY(Entradas->entry[i]), date);
    gtk_box_pack_start(GTK_BOX(horizontales[i]), label[i], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(horizontales[i]), Entradas->entry[i], TRUE, TRUE, 20);
    gtk_box_pack_start(GTK_BOX(vertical), horizontales[i], TRUE, TRUE, 0);
  }
  boton = gtk_button_new_from_stock(GTK_STOCK_OK);
  gtk_signal_connect(GTK_OBJECT(boton), "clicked", GTK_SIGNAL_FUNC(crearCita), (gpointer)Entradas);
  invisible[0] = gtk_label_new(NULL);
  invisible[1] = gtk_label_new(NULL);
  gtk_box_pack_start(GTK_BOX(horizontales[6]),invisible[0],TRUE,TRUE, 0);
  gtk_box_pack_start(GTK_BOX(horizontales[6]),boton,TRUE,TRUE, 0);
  gtk_box_pack_start(GTK_BOX(horizontales[6]),invisible[1],TRUE,TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vertical), horizontales[6],TRUE,TRUE, 0);
  invisible[2] = gtk_label_new(NULL);
  gtk_box_pack_start(GTK_BOX(vertical), invisible[2],TRUE,TRUE, 0);
  gtk_container_add(GTK_CONTAINER(Entradas->window), vertical);
  gtk_widget_show_all(Entradas->window);
  gtk_main();
  g_free(Entradas);
}
void getDate(char date[]){
  int dd, mm, aa;
  char Timestamp[200];
  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(Timestamp,200, "%Y,%m,%d", timeinfo);
  sscanf(Timestamp,"%d,%d,%d",&aa,&mm,&dd);
  sprintf(date, "%d/%d/%d", dd, mm, aa);
}
void crearCita(GtkWidget* boton, gpointer data){
  GenerarHistoria* datos = (GenerarHistoria*)data;
  Historia *Nuevo = (Historia*)malloc(sizeof(Historia));
  GtkWidget* dialog;
  int bandera = 0;
  Pacientes* temp = datos->ListaPacientes;
  Pacientes* temp2;
  const gchar* paciente, *doctor, *fechaCita, *diagnostico, *tratamiento, *anotaciones;
  paciente = gtk_entry_get_text(GTK_ENTRY(datos->entry[0]));
  doctor = gtk_entry_get_text(GTK_ENTRY(datos->entry[1]));
  fechaCita = gtk_entry_get_text(GTK_ENTRY(datos->entry[2]));
  diagnostico = gtk_entry_get_text(GTK_ENTRY(datos->entry[3]));
  tratamiento = gtk_entry_get_text(GTK_ENTRY(datos->entry[4]));
  anotaciones = gtk_entry_get_text(GTK_ENTRY(datos->entry[5]));
  while (temp != NULL && bandera == 0) {
    if(strcmp(paciente, temp->Nombre) == 0){
      bandera = 1;
      temp2 = temp;
    }
    temp = temp->sig;
  }
  if(bandera == 0){
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "El paciente no ha sido encontrado en la base de datos):");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    gtk_widget_destroy(datos->window);
    return;
  }
  if(diagnostico[0] == '\0' || tratamiento[0] == '\0' || anotaciones[0] == '\0'){
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Faltan campos por llenar");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }
  Nuevo->id[0] = '0';
  Nuevo->id[1] = '\0';
  strcpy(Nuevo->NombrePaciente, paciente);
  strcpy(Nuevo->NombreDoctor, doctor);
  strcpy(Nuevo->FechaCita, fechaCita);
  strcpy(Nuevo->Diagnostico, diagnostico);
  strcpy(Nuevo->Tratamiento, tratamiento);
  strcpy(Nuevo->Anotaciones, anotaciones);
  Nuevo->sig = NULL;
  agregarNodoHistoria(Nuevo, &temp2);

  actualizarArchivoHistoria(temp2->HClinica, paciente);
  dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Se ha generado la cita de manera exitosa");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
  gtk_widget_destroy(datos->window);
}
void actualizarArchivoHistoria(Historia* HClinica, const gchar* paciente){
  char NombreArchivo[100];
  int i = 0;
  FILE* Archivo;
  Historia* temp = HClinica;
  strcpy(NombreArchivo, paciente);
  while (NombreArchivo[i] != '\0') {
    if(NombreArchivo[i] == ' ')
      NombreArchivo[i] = '_';
    i++;
  }
  strcat(NombreArchivo, ".txt");
  Archivo = fopen(NombreArchivo, "wt");
  while (temp != NULL) {
    fprintf(Archivo, " %s\n", temp->id);
    fprintf(Archivo, " %s\n", temp->NombrePaciente);
    fprintf(Archivo, " %s\n", temp->NombreDoctor);
    fprintf(Archivo, " %s\n", temp->FechaCita);
    fprintf(Archivo, " %s\n", temp->Diagnostico);
    fprintf(Archivo, " %s\n", temp->Tratamiento);
    fprintf(Archivo, " %s\n", temp->Anotaciones);
    temp = temp->sig;
  }
  fclose(Archivo);
}
void agregarNodoHistoria(Historia* Nuevo, Pacientes** temp){
  Historia* temp3;
  if((*temp)->HClinica == NULL){
    (*temp)->HClinica = Nuevo;
  }else{
    temp3 = (*temp)->HClinica;
    while (temp3->sig != NULL) {
      temp3 = temp3->sig;
    }
    temp3->sig = Nuevo;
  }
}
void mostrarHistorial(ParametrosListas *temp,const gchar* nombre){
  Pacientes *paciente = temp->ListaPacientes;
  int flag = 0, numCita = 1, i = 0;
  Historia* clinica;
  GtkWidget *window, *swin, *viewport, *table1, *vbox, *label[10], *labelTit, *dialog;
  GtkAdjustment *horizontal, *vertical;
  char titulo[200], auxiliar[200], aux2[200];
  PangoAttrList *attrlist = pango_attr_list_new();
  PangoAttribute *attr = pango_attr_size_new_absolute(20 * PANGO_SCALE);
  pango_attr_list_insert(attrlist, attr);
  while(paciente != NULL && flag == 0){
  	if(strcmp(paciente->Nombre, nombre) == 0)
  		flag = 1;
  	else
  		paciente = paciente->sig;
  }
  if(flag == 0 || paciente->HClinica == NULL){
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "No se ha encontrado la historia médica de este paciente");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }
  clinica = paciente->HClinica;
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Historial Médico");
  gtk_container_set_border_width (GTK_CONTAINER (window), 10);
  gtk_widget_set_size_request (window, 500, 400);
  g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
  strcpy(titulo, "Historial Médico de ");
  strcat(titulo, nombre);
  labelTit = gtk_label_new (titulo);
  gtk_misc_set_alignment (GTK_MISC(labelTit), 0, 0.5);
  pango_attr_list_insert(attrlist, attr);
  gtk_label_set_attributes(GTK_LABEL(labelTit), attrlist);
  table1 = gtk_table_new (5, 5, FALSE);
  gtk_table_set_row_spacings (GTK_TABLE (table1), 0);
  gtk_table_set_col_spacings (GTK_TABLE (table1), 0);
  while (clinica != NULL) {
    strcpy(auxiliar, "Cita número ");
    sprintf(aux2, "%d", numCita);
    strcat(auxiliar, aux2);
    strcat(auxiliar, ": ");
    label[0] = gtk_label_new(auxiliar);
    gtk_misc_set_alignment (GTK_MISC(label[0]), 0, 0.3);
    pango_attr_list_insert(attrlist, attr);
    gtk_label_set_attributes(GTK_LABEL(label[0]), attrlist);
    gtk_table_attach_defaults (GTK_TABLE(table1), label[0], 0, 1, i, i+1);
    auxiliar[0] = '\0';
    label[6] = gtk_label_new(NULL);
    i+=2;
    gtk_table_attach_defaults (GTK_TABLE(table1), label[6], 0, 1, i, i+1);
    i+= 2;
    strcpy(auxiliar, "Doctor que atendió: ");
    strcat(auxiliar, clinica->NombreDoctor);
    label[1] = gtk_label_new(auxiliar);
    gtk_misc_set_alignment (GTK_MISC(label[1]), 0, 0);
    gtk_table_attach_defaults(GTK_TABLE(table1), label[1], 0, 1, i, i+1);
    i+= 2;
    auxiliar[0] = '\0';
    strcpy(auxiliar, "Fecha de la cita: ");
    strcat(auxiliar, clinica->FechaCita);
    label[2] = gtk_label_new(auxiliar);
    gtk_misc_set_alignment (GTK_MISC(label[2]), 0, 0);
    gtk_table_attach_defaults (GTK_TABLE(table1), label[2], 0, 1, i, i+1);
    i+= 2;
    auxiliar[0] = '\0';
    strcpy(auxiliar, "Diagnostico: ");
    strcat(auxiliar, clinica->Diagnostico);
    label[3] = gtk_label_new(auxiliar);
    gtk_misc_set_alignment (GTK_MISC(label[3]), 0, 0);
    gtk_table_attach_defaults (GTK_TABLE(table1), label[3], 0, 1, i, i+1);
    i+= 2;
    auxiliar[0] = '\0';
    strcpy(auxiliar, "Tratamiento: ");
    strcat(auxiliar, clinica->Tratamiento);
    label[4] = gtk_label_new(auxiliar);
    gtk_misc_set_alignment (GTK_MISC(label[4]), 0, 0);
    gtk_table_attach_defaults (GTK_TABLE(table1), label[4], 0, 1, i, i+1);
    i+= 2;
    auxiliar[0] = '\0';
    strcpy(auxiliar, "Anotaciones: ");
    strcat(auxiliar, clinica->Anotaciones);
    label[5] = gtk_label_new(auxiliar);
    gtk_misc_set_alignment (GTK_MISC(label[5]), 0, 0);
    gtk_table_attach_defaults (GTK_TABLE(table1), label[5], 0, 1, i, i+1);
    label[6] = gtk_label_new(NULL);
    i+=2;
    gtk_table_attach_defaults (GTK_TABLE(table1), label[6], 0, 1, i, i+1);
    clinica = clinica->sig;
    i+= 2;
    numCita++;
  }
  swin = gtk_scrolled_window_new (NULL, NULL);
  horizontal = gtk_scrolled_window_get_hadjustment (GTK_SCROLLED_WINDOW (swin));
  vertical = gtk_scrolled_window_get_vadjustment (GTK_SCROLLED_WINDOW (swin));
  viewport = gtk_viewport_new (horizontal, vertical);
  gtk_container_set_border_width (GTK_CONTAINER (swin), 5);
  gtk_container_set_border_width (GTK_CONTAINER (viewport), 5);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (swin), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (swin), table1);
  vbox = gtk_vbox_new (FALSE, 5);
  gtk_box_pack_start (GTK_BOX (vbox), labelTit, FALSE, FALSE, 20);
  gtk_box_pack_start_defaults (GTK_BOX (vbox), swin);
  gtk_container_add (GTK_CONTAINER (window), vbox);

  gtk_widget_show_all (window);
  gtk_main();
}
void DesplegarListaPacientes(GtkWidget* menu, gpointer Listas){
  ParametrosListas* datos = (ParametrosListas*)Listas;
  Pacientes* ListaPacientes = datos->ListaPacientes;
  GtkWidget* dialog, *window, *labelTit, *table1, *vbox, *swin, *viewport, *label[10];
  GtkAdjustment *horizontal, *vertical;
  int i = 0, numPaciente = 0;
  char auxiliar[200], aux2[200];
  if(ListaPacientes == NULL){
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Aún no hay pacientes registrados");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }
  PangoAttrList *attrlist = pango_attr_list_new();
  PangoAttribute *attr = pango_attr_size_new_absolute(20 * PANGO_SCALE);
  pango_attr_list_insert(attrlist, attr);
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Lista de pacientes");
  gtk_container_set_border_width (GTK_CONTAINER (window), 10);
  gtk_widget_set_size_request (window, 500, 400);
  g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);

  labelTit = gtk_label_new ("Lista de pacientes");
  gtk_misc_set_alignment (GTK_MISC(labelTit), 0, 0.5);
  pango_attr_list_insert(attrlist, attr);
  gtk_label_set_attributes(GTK_LABEL(labelTit), attrlist);
  table1 = gtk_table_new (5, 5, FALSE);
  gtk_table_set_row_spacings (GTK_TABLE (table1), 0);
  gtk_table_set_col_spacings (GTK_TABLE (table1), 0);

  while (ListaPacientes != NULL) {
    strcpy(auxiliar, "Paciente número ");
    sprintf(aux2, "%d", numPaciente+1 );
    strcat(auxiliar, aux2);
    strcat(auxiliar, ": ");
    label[0] = gtk_label_new(auxiliar);
    gtk_misc_set_alignment (GTK_MISC(label[0]), 0, 0);
    pango_attr_list_insert(attrlist, attr);
    gtk_label_set_attributes(GTK_LABEL(label[0]), attrlist);
    gtk_table_attach_defaults (GTK_TABLE(table1), label[0], 0, 1, i, i+1);
    auxiliar[0] = '\0';
    strcpy(auxiliar, ListaPacientes->Nombre);
    strcat(auxiliar, ".  (");
    strcat(auxiliar, ListaPacientes->telefono);
    strcat(auxiliar, ").");
    label[1] = gtk_label_new(auxiliar);
    gtk_misc_set_alignment (GTK_MISC(label[1]), 0, 0);
    gtk_table_attach_defaults(GTK_TABLE(table1), label[1], 1, 2, i, i+1);
    i+= 2;
    auxiliar[0] = '\0';
    ListaPacientes = ListaPacientes->sig;
    numPaciente++;
  }

  swin = gtk_scrolled_window_new (NULL, NULL);
  horizontal = gtk_scrolled_window_get_hadjustment (GTK_SCROLLED_WINDOW (swin));
  vertical = gtk_scrolled_window_get_vadjustment (GTK_SCROLLED_WINDOW (swin));
  viewport = gtk_viewport_new (horizontal, vertical);
  gtk_container_set_border_width (GTK_CONTAINER (swin), 5);
  gtk_container_set_border_width (GTK_CONTAINER (viewport), 5);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (swin), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (swin), table1);
  vbox = gtk_vbox_new (FALSE, 5);
  gtk_box_pack_start (GTK_BOX (vbox), labelTit, FALSE, FALSE, 20);
  gtk_box_pack_start_defaults (GTK_BOX (vbox), swin);
  gtk_container_add (GTK_CONTAINER (window), vbox);


  gtk_widget_show_all (window);
  gtk_main();
}

void interfazDoctores(GtkWidget* item, gpointer Parametros){
  ParametrosListas* Listas = (ParametrosListas*)Parametros;
  gtk_widget_destroy(Listas->window);
  Listas->sexo = 0;
  PangoAttrList *attrlist = pango_attr_list_new();
  PangoAttribute *attr = pango_attr_size_new_absolute(20 * PANGO_SCALE);
  GtkWidget* menuP, *vertical, *horizontales[11], *label[20], *invisible[11], *boton, *horizontalA, *botonesA[5], *botonLimpiar, *containerDown, *titulo, *cajatitulo;
  char campos[11][200];
  copiarStrings2(campos);
  // Creando las cajas
  containerDown = gtk_hbox_new(TRUE, 10);
  vertical = gtk_vbox_new(0, 0);
  for(int i = 0; i < 12; i++){
    if(i == 3)
      horizontales[i] = gtk_hbox_new(FALSE, 37);
    else
      horizontales[i] = gtk_hbox_new(TRUE, 0);
  }
  horizontalA = gtk_hbox_new(TRUE, 2);
  // Creando ventana principal
  Listas->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(Listas->window), "Sistema de información médica");
  gtk_widget_set_size_request(Listas->window, 600, 650);
  gtk_window_set_resizable(GTK_WINDOW(Listas->window), FALSE);
  gtk_window_set_position(GTK_WINDOW(Listas->window), GTK_WIN_POS_CENTER_ALWAYS);
  gtk_signal_connect(GTK_OBJECT(Listas->window), "destroy", GTK_SIGNAL_FUNC(destroy), NULL);
  menuP = menu(Listas); // Creando el menu
  gtk_box_pack_start(GTK_BOX(vertical), menuP, 0, 0, 0);

  cajatitulo = gtk_hbox_new (TRUE, 0); //Creando la caja del encabezado
  titulo = gtk_label_new ("Administración de doctores"); //Creando el encabezado
  gtk_misc_set_alignment (GTK_MISC(titulo), 0.05, 0.5); //Alineando a la izquierda
  pango_attr_list_insert(attrlist, attr); //Llamando a la lista de atributos
  gtk_label_set_attributes(GTK_LABEL(titulo), attrlist); //Asignando atributos a título
  gtk_box_pack_start (GTK_BOX (cajatitulo), titulo, TRUE, TRUE, 0); //Título -> CajaTitulo
  gtk_box_pack_start (GTK_BOX (vertical), cajatitulo, TRUE, TRUE, 20); //CajaTitulo -> Vertical

  // Elementos principales de la interfaz
  for(int i = 0; i < 12; i++){
    if(i < 11){
      if(i == 3 || i == 4){
        if(i == 3){
          label[i] = gtk_label_new("Status: ");
	         gtk_box_pack_start(GTK_BOX(horizontales[i]), label[i], TRUE, TRUE, 0);
        }
        if(i == 3){
          Listas->entry[i] = gtk_radio_button_new_with_label(NULL, "Activo");
          gtk_box_pack_start(GTK_BOX(horizontales[3]), Listas->entry[i], FALSE, FALSE, 0);
        }else{
          Listas->entry[i] = gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(Listas->entry[3])), "Inactivo");
          gtk_box_pack_start(GTK_BOX(horizontales[3]), Listas->entry[i], FALSE, FALSE, 0);
          label[i] = gtk_label_new(NULL);
          gtk_box_pack_start(GTK_BOX(horizontales[3]), label[i], TRUE, TRUE, 20);
        }
        gtk_signal_connect(GTK_OBJECT(Listas->entry[i]), "toggled", GTK_SIGNAL_FUNC(radio), (gpointer)Listas);
      }else{
        label[i] = gtk_label_new(campos[i]);
        gtk_box_pack_start(GTK_BOX(horizontales[i]), label[i], TRUE, TRUE, 0);
        if(i == 1 || i == 2){
          Listas->entry[i] = gtk_combo_box_new_text();
          gtk_combo_box_append_text(GTK_COMBO_BOX(Listas->entry[i]), "Anatomía");
          gtk_combo_box_append_text(GTK_COMBO_BOX(Listas->entry[i]), "Cirugía General");
          gtk_combo_box_append_text(GTK_COMBO_BOX(Listas->entry[i]), "Ginecología y Obstetricia");
          gtk_combo_box_append_text(GTK_COMBO_BOX(Listas->entry[i]), "Medicina General");
          gtk_combo_box_append_text(GTK_COMBO_BOX(Listas->entry[i]), "Ortopedia");
          gtk_combo_box_append_text(GTK_COMBO_BOX(Listas->entry[i]), "Pediatria");
          gtk_combo_box_append_text(GTK_COMBO_BOX(Listas->entry[i]), "Otorrinolaringólogo");
          gtk_combo_box_append_text(GTK_COMBO_BOX(Listas->entry[i]), "Radiología e imágen");
          gtk_combo_box_set_active(GTK_COMBO_BOX(Listas->entry[i]), 0);
        }else{
          if(i == 9){
            Listas->entry[i] = gtk_combo_box_new_text();
            gtk_combo_box_append_text(GTK_COMBO_BOX(Listas->entry[i]), "07:00 - 15:00");
            gtk_combo_box_append_text(GTK_COMBO_BOX(Listas->entry[i]), "15:00 - 23:00");
            gtk_combo_box_append_text(GTK_COMBO_BOX(Listas->entry[i]), "23:00 - 07:00");
            gtk_combo_box_set_active(GTK_COMBO_BOX(Listas->entry[i]), 0);
          }else
            Listas->entry[i] = gtk_entry_new();
        }
        gtk_box_pack_start(GTK_BOX(horizontales[i]), Listas->entry[i], TRUE, TRUE, 0);
      }

      if(i > 4){
        invisible[i] = gtk_label_new(NULL);
        gtk_box_pack_start(GTK_BOX(horizontales[i]), invisible[i], TRUE, TRUE, 0);
      }
      if(i > 0  && i <4){
        if(i == 1){
          botonesA[i] = AddButton(horizontales[i], "Actualizar", botonesControlA, i+1);
          gtk_signal_connect(GTK_OBJECT(botonesA[i]), "clicked", GTK_SIGNAL_FUNC(botonesControlDoc), (gpointer)Listas);
        }
        if(i == 2){
          botonesA[i] = AddButton(horizontales[i], "Actualizar", botonesControlA, 4);
          gtk_signal_connect(GTK_OBJECT(botonesA[i]), "clicked", GTK_SIGNAL_FUNC(botonesControlDoc), (gpointer)Listas);
        }
      }
      gtk_box_pack_start(GTK_BOX(vertical), horizontales[i], TRUE, TRUE, 0);
    }
    if(i == 11){
      label[11] = gtk_label_new("Telefono de urgencias");
      gtk_box_pack_start(GTK_BOX(horizontales[i]), label[i], TRUE, TRUE, 0);
      Listas->entry[i] = gtk_entry_new();
      gtk_box_pack_start(GTK_BOX(horizontales[i]), Listas->entry[i], TRUE, TRUE, 0);
      label[11] = gtk_label_new(NULL);
      gtk_box_pack_start(GTK_BOX(horizontales[i]), label[i], TRUE, TRUE, 0);
      gtk_box_pack_start(GTK_BOX(vertical), horizontales[i], TRUE, TRUE, 0);
    }
  }

  // Creando boton de busqueda
  boton = AddButton(horizontales[0], "Buscar", buscar, 1);
  gtk_signal_connect(GTK_OBJECT(boton), "clicked", GTK_SIGNAL_FUNC(buscar2), (gpointer)Listas);
  botonLimpiar = AddButton(containerDown, "Act", botonesControlA, 6);
  gtk_signal_connect(GTK_OBJECT(botonLimpiar), "clicked", GTK_SIGNAL_FUNC(limpiarCampos2), (gpointer)Listas);


  invisible[10] = gtk_label_new(NULL);
  gtk_box_pack_start(GTK_BOX(vertical), horizontalA, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vertical), invisible[10], TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vertical), containerDown, TRUE, TRUE, 0);
  invisible[10] = gtk_label_new(NULL);
  gtk_box_pack_start(GTK_BOX(vertical), invisible[10], TRUE, TRUE, 0);
  gtk_container_add(GTK_CONTAINER(Listas->window), vertical);
  gtk_widget_show_all(Listas->window);
  gtk_main();
}

void copiarStrings2(char campos[11][200]){
  strcpy(campos[0], "Nombre del doctor: ");
  strcpy(campos[1], "Especialidad 1: ");
  strcpy(campos[2], "Especialidad 2: ");
  strcpy(campos[3], "status: ");
  strcpy(campos[4], "Teléfono de urgencias: ");
  strcpy(campos[6], "Dirección: ");
  strcpy(campos[5], "Teléfono: ");
  strcpy(campos[7], "Consultorio asignado: ");
  strcpy(campos[8], "Dias de consulta: ");
  strcpy(campos[9], "Horario de consulta: ");
  strcpy(campos[10], "Contraseña: ");
}

void limpiarCampos2(GtkButton *button, gpointer data){
  ParametrosListas* datos = (ParametrosListas*)data;
  datos->nombreBuscado[0] = '\0';
  for(int i = 0; i < 11; i++)
    if(i != 4 && i != 3 && i != 9 && i != 1 && i != 2)
      gtk_entry_set_text(GTK_ENTRY(datos->entry[i]), "");
  gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[9]), 0);
}

void buscar2(GtkWidget* widget, gpointer data){
  GtkWidget *dialog;
  const gchar* nombre;
  int flag = 0;
  ParametrosListas* datos = (ParametrosListas*)data;
  Doctores* temp = datos->ListaDoctores;
  datos->flag = 0;
  nombre = gtk_entry_get_text(GTK_ENTRY(datos->entry[0]));
  if(temp == NULL){
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Aún no hay doctores");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
  }else{
    while (temp != NULL && flag == 0) {
      if(strcmp(nombre, temp->FullName) == 0)
        flag = 1;
      else
        temp = temp->sig;
    }
    if(flag == 0){
      dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "No se ha encontrado el doctor");
      gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
      datos->nombreBuscado[0] = '\0';
      gtk_entry_set_text(GTK_ENTRY(datos->entry[5]), "");
      gtk_entry_set_text(GTK_ENTRY(datos->entry[6]), "");
      gtk_entry_set_text(GTK_ENTRY(datos->entry[7]), "");
      gtk_entry_set_text(GTK_ENTRY(datos->entry[8]), "");
      gtk_entry_set_text(GTK_ENTRY(datos->entry[10]), "");
    }else{
      dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Se ha encontrado con éxito al doctor");
      gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
      gtk_entry_set_text(GTK_ENTRY(datos->entry[0]), temp->FullName);
      strcpy(datos->nombreBuscado, temp->FullName);
      if(strcmp(temp->Especialidad1, "Anatomía") == 0){
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[1]), 0);
      }
      if(strcmp(temp->Especialidad1, "Cirugía General") == 0){
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[1]), 1);
      }
      if(strcmp(temp->Especialidad1, "Ginecología y Obstetricia") == 0){
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[1]), 2);
      }
      if(strcmp(temp->Especialidad1, "Medicina General") == 0){
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[1]), 3);
      }
      if(strcmp(temp->Especialidad1, "Ortopedia") == 0){
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[1]), 4);
      }
      if(strcmp(temp->Especialidad1, "Pediatria") == 0){
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[1]), 5);
      }
      if(strcmp(temp->Especialidad1, "Otorrinolaringólogo") == 0){
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[1]), 6);
      }
      if(strcmp(temp->Especialidad1, "Radiología e imágen") == 0){
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[1]), 7);
      }

      if(strcmp(temp->Especialidad2, "Anatomía") == 0){
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[2]), 0);
      }
      if(strcmp(temp->Especialidad2, "Cirugía General") == 0){
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[2]), 1);
      }
      if(strcmp(temp->Especialidad2, "Ginecología y Obstetricia") == 0)
          gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[2]), 2);
      if(strcmp(temp->Especialidad2, "Medicina General") == 0){
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[2]), 3);
      }
      if(strcmp(temp->Especialidad2, "Ortopedia") == 0){
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[2]), 4);
      }
      if(strcmp(temp->Especialidad2, "Pediatria") == 0){
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[2]), 5);
      }
      if(strcmp(temp->Especialidad2, "Otorrinolaringólogo") == 0){
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[2]), 6);
      }
      if(strcmp(temp->Especialidad2, "Radiología e imágen") == 0){
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[2]), 7);
      }
      if(temp->status == 0){
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(datos->entry[3]), TRUE);
      }
      if(temp->status == 1){
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(datos->entry[4]), TRUE);
      }
      gtk_entry_set_text(GTK_ENTRY(datos->entry[5]), temp->telefono);
      gtk_entry_set_text(GTK_ENTRY(datos->entry[6]), temp->Direccion);
      gtk_entry_set_text(GTK_ENTRY(datos->entry[7]), temp->ConsultorioAsignado);
      gtk_entry_set_text(GTK_ENTRY(datos->entry[8]), temp->diasConsulta);
      gtk_entry_set_text(GTK_ENTRY(datos->entry[10]), temp->Password);
      gtk_entry_set_text(GTK_ENTRY(datos->entry[11]), temp->telefonoUrgencias);
      if(strcmp(temp->HorarioConsulta, "07:00 - 15:00") == 0)
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[9]), 0);

      if(strcmp(temp->HorarioConsulta, "15:00 - 23:00") == 0)
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[9]), 0);

      if(strcmp(temp->HorarioConsulta, "23:00 - 07:00") == 0)
        gtk_combo_box_set_active(GTK_COMBO_BOX(datos->entry[9]), 0);

    }
  }
}
void botonesControlDoc(GtkWidget* button, gpointer data){
 ParametrosListas* datos = (ParametrosListas*)data;
  int validacion[7];
  char boton[200];
  int status, comboActive;
  GtkWidget* dialog;
  const gchar* nombre, *especialidad1, *especialidad2, *telefono, *direccion, *consultorio, *dias, *horario, *contra, *telefonoUrgencias;
  strcpy(boton, gtk_widget_get_name(GTK_WIDGET(gtk_button_get_image(GTK_BUTTON(button)))));
  nombre = gtk_entry_get_text(GTK_ENTRY(datos->entry[0]));
  comboActive = gtk_combo_box_get_active(GTK_COMBO_BOX(datos->entry[1]));
  switch (comboActive) {
    case 0:
      especialidad1 = "Anatomía";
      break;
    case 1:
      especialidad1 = "Cirugía General";
      break;
    case 2:
      especialidad1 = "Ginecología y Obstetricia";
      break;
    case 3:
      especialidad1 = "Medicina General";
      break;
    case 4:
      especialidad1 = "Ortopedia";
      break;
    case 5:
      especialidad1 = "Pediatria";
      break;
    case 6:
      especialidad1 = "Otorrinolaringólogo";
      break;
    case 7:
      especialidad1 = "Radiología e imágen";
      break;
  }
  comboActive = gtk_combo_box_get_active(GTK_COMBO_BOX(datos->entry[2]));
  switch (comboActive) {
    case 0:
      especialidad2 = "Anatomía";
      break;
    case 1:
      especialidad2 = "Cirugía General";
      break;
    case 2:
      especialidad2 = "Ginecología y Obstetricia";
      break;
    case 3:
      especialidad2 = "Medicina General";
      break;
    case 4:
      especialidad2 = "Ortopedia";
      break;
    case 5:
      especialidad2 = "Pediatria";
      break;
    case 6:
      especialidad2 = "Otorrinolaringólogo";
      break;
    case 7:
      especialidad2 = "Radiología e imágen";
      break;
  }
  status = datos->sexo;
  telefono = gtk_entry_get_text(GTK_ENTRY(datos->entry[5]));
  direccion = gtk_entry_get_text(GTK_ENTRY(datos->entry[6]));
  consultorio = gtk_entry_get_text(GTK_ENTRY(datos->entry[7]));
  dias = gtk_entry_get_text(GTK_ENTRY(datos->entry[8]));
  contra = gtk_entry_get_text(GTK_ENTRY(datos->entry[10]));
  telefonoUrgencias = gtk_entry_get_text(GTK_ENTRY(datos->entry[11]));
  comboActive = gtk_combo_box_get_active(GTK_COMBO_BOX(datos->entry[9]));
  switch (comboActive) {
  case 0:
    horario = "07:00 - 15:00";
    break;
  case 1:
    horario = "15:00 - 23:00";
    break;
  case 2:
    horario = "23:00 - 07:00";
    break;
  }
  validacion[0] = validarLetras(nombre, "Nombre doctor");
  validacion[1] = validarNumeros(telefono, "Telefono");
  validacion[2] = validarLetras(dias, "Dias de consulta");
  validacion[3] = validarNumeros(telefonoUrgencias, "Telefono de urgencias");
  validacion[4] = direccion[0] == '\0'? 1: 0;
  validacion[5] = consultorio[0] == '\0'? 1: 0;
  if(validacion[4] == 1 || validacion[5] == 1){
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Aún faltan campos por llenar");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }
  for(int i = 0; i < 4; i++)
    if(validacion[i] == 1)
      return;

  if(strcmp("Actualizar", boton) == 0)
    modificarDoctor(nombre, direccion, telefono, status, consultorio, dias, contra, telefonoUrgencias, horario, especialidad1, especialidad2, datos->ListaDoctores);
  if(strcmp("NuevoPaciente", boton) == 0){
    nuevoDoctor(nombre, direccion, telefono, status, consultorio, dias, contra, telefonoUrgencias, horario, especialidad1, especialidad2, &datos->ListaDoctores);
  }
}
void modificarDoctor(const gchar* nombre, const gchar* direccion, const gchar* telefono, int status, const gchar* consultorio, const gchar* dias, const gchar* contra, const gchar* telefonoUrgencias, const gchar* horario, const gchar* especialidad1, const gchar* especialidad2, Doctores* ListaDoctores){
  Doctores* temp = ListaDoctores;
  GtkWidget* dialog;
  while (temp != NULL && strcmp(nombre, temp->FullName) != 0) {
    temp = temp->sig;
  }
  if(temp == NULL){
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "El doctor no se encuentra en la base de datos");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }
  strcpy(temp->FullName, nombre);
  strcpy(temp->Direccion, direccion);
  strcpy(temp->telefono, telefono);
  temp->status = status;
  strcpy(temp->ConsultorioAsignado, consultorio);
  strcpy(temp->diasConsulta, dias);
  strcpy(temp->Password, contra);
  strcpy(temp->telefonoUrgencias, telefonoUrgencias);
  strcpy(temp->HorarioConsulta, horario);
  strcpy(temp->Especialidad1, especialidad1);
  strcpy(temp->Especialidad2, especialidad2);

  actualizarArchivoDoctores(ListaDoctores);
  dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "El doctor ha sido actualizado con éxito");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}
void actualizarArchivoDoctores(Doctores* ListaDoctores){
  Doctores* temp = ListaDoctores;
  FILE* Archivo = fopen("doctores.txt", "wt");
  while(temp != NULL){
    fprintf(Archivo, "%s\n", temp->FullName);
    fprintf(Archivo, "%s\n", temp->Especialidad1);
    fprintf(Archivo, "%s\n", temp->Especialidad2);
    fprintf(Archivo, "%d\n", temp->status);
    fprintf(Archivo, "%s\n", temp->telefonoUrgencias);
    fprintf(Archivo, "%s\n", temp->Direccion);
    fprintf(Archivo, "%s\n", temp->telefono);
    fprintf(Archivo, "%s\n", temp->ConsultorioAsignado);
    fprintf(Archivo, "%s\n", temp->diasConsulta);
    fprintf(Archivo, "%s\n", temp->HorarioConsulta);
    fprintf(Archivo, "%s\n", temp->Password);
    temp = temp->sig;
  }
  fclose(Archivo);
}
void nuevoDoctor(const gchar* nombre, const gchar* direccion, const gchar* telefono, int status, const gchar* consultorio, const gchar* dias, const gchar* contra, const gchar* telefonoUrgencias, const gchar* horario, const gchar* especialidad1, const gchar* especialidad2, Doctores** ListaDoctores){
  Doctores* temp2 = *ListaDoctores;
  Doctores* temp3 = *ListaDoctores;
  Doctores* temp4 = *ListaDoctores;
  Doctores* temp = (Doctores*)malloc(sizeof(Doctores));
  GtkWidget* dialog;
  int Bandera = 0;
  while (temp3 != NULL && Bandera == 0) {
    if(strcmp(temp3->FullName, nombre) == 0)
      Bandera = 1;
    temp3 = temp3->sig;
  }
  if(Bandera == 1){
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "No se puede agregar este paciente debido a que ya existe uno con el mismo nombre");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }
  strcpy(temp->FullName, nombre);
  strcpy(temp->Direccion, direccion);
  strcpy(temp->telefono, telefono);
  temp->status = status;
  strcpy(temp->ConsultorioAsignado, consultorio);
  strcpy(temp->diasConsulta, dias);
  strcpy(temp->Password, contra);
  strcpy(temp->telefonoUrgencias, telefonoUrgencias);
  strcpy(temp->HorarioConsulta, horario);
  strcpy(temp->Especialidad1, especialidad1);
  strcpy(temp->Especialidad2, especialidad2);
  temp->sig = NULL;
  while((temp2!=NULL) && (strcmp(nombre,temp2->FullName)>0))
    temp2=temp2->sig;

  if(temp2!=NULL){
      if(temp2 != *ListaDoctores){
        while(temp4->sig!=temp2)
          temp4=temp4->sig;
        temp->sig=temp2;
        temp4->sig=temp;
      }else
        Inserta_Inicio2(ListaDoctores,temp);
  }else
    Inserta_Fin2(ListaDoctores,temp);

  actualizarArchivoDoctores(*ListaDoctores);
  dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Se ha agregado de manera correcta el doctor");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void Inserta_Inicio2(Doctores **ListaDoctores, Doctores *nuevo){
  nuevo->sig=*ListaDoctores;
  *ListaDoctores=nuevo;
}
void Inserta_Fin2(Doctores **ListaDoctores, Doctores *nuevo){
  Doctores *temp2;
  nuevo->sig=NULL;
  if(*ListaDoctores!=NULL){
      temp2=*ListaDoctores;
      while(temp2->sig!=NULL)
	     temp2=temp2->sig;
      temp2->sig=nuevo;
    }
  else
    *ListaDoctores=nuevo;
}
