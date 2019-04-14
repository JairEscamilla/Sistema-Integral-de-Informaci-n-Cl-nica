// Incluyendo las funciones
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
// Estructuras de datos
typedef struct defDoctores{ // Estructura definida para los datos de un doctor
  char FullName[200];
  char Especialidad1[200];
  char Especialidad2[200];
  int status;// 1-> Activo y 0-> Inactivo
  int telefonoUrgencias;
  char Direccion[200];
  int telefono;
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
  char id[10];
  char Nombre[200];
  char Direccion[200];
  int telefono;
  char sexo;
  int dia;
  int mes;
  int anio;
  int edad;
  float estatura;
  char alergias[200];
  char tipoSangre[10];
  char PadecimientosCronicos[200];
  struct defPacientes* sig;
  Historia* HClinica;
}Pacientes;
typedef struct _defParametrosLogin{
  GtkWidget* entry[2], *window;
  Doctores* Lista;
  int bandera;
}Login;
// Prototipos de las funciones
void leerListaDoctores(Doctores**);
void leerListaPacientes(Pacientes**);
void leerHistorial(Pacientes*);
void destroy(GtkWidget* wideget, gpointer data);
GtkWidget *AddButton(GtkWidget *theBox, const gchar *buttonText, gpointer CallBackFunction);
void iniciarSesion(GtkButton *button, gpointer data);
void loger(Doctores*, Login*, int*);
// Función principal
int main(int argc, char *argv[]) {
  Doctores* ListaDoctores = NULL;
  Pacientes* ListaPacientes = NULL;
  Login* Parametros = (Login*)malloc(sizeof(Login));
  gtk_init(&argc, &argv);
  int h = 0;
  leerListaDoctores(&ListaDoctores); // Leyendo Doctores
  leerListaPacientes(&ListaPacientes); // Leyendo Pacientes
  leerHistorial(ListaPacientes); // Historia clinica de caada paciente
  loger(ListaDoctores, Parametros, &h);
  if(h == 1){
    printf("Entrando al sistema\n");
  }
  g_free(Parametros);
  return 0;
}
// Desarrollando las funciones

void loger(Doctores* Lista, Login* Parametros, int* band){
  // Creando ventana para login
  Parametros->Lista = Lista;
  Parametros->bandera = 0;
  GtkWidget* label, *horizontal, *horizontal2, *horizontal3, *label2, *vertical, *boton;
  Parametros->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(Parametros->window), "Sistema de información médica");
  gtk_widget_set_size_request(Parametros->window, 300, 200);
  gtk_window_set_resizable(GTK_WINDOW(Parametros->window), FALSE);
  gtk_container_border_width(GTK_CONTAINER(Parametros->window), 15);
  gtk_window_set_position(GTK_WINDOW(Parametros->window), GTK_WIN_POS_CENTER_ALWAYS);
  gtk_signal_connect(GTK_OBJECT(Parametros->window), "destroy", GTK_SIGNAL_FUNC(destroy), NULL);
  // Creando las cajas
  horizontal = gtk_hbox_new(TRUE, 5);
  horizontal2 = gtk_hbox_new(TRUE, 5);
  horizontal3 = gtk_hbox_new(TRUE, 5);
  vertical = gtk_vbox_new(TRUE, 2);
  // Agregando un label
  label = gtk_label_new("Nombre: ");
  gtk_box_pack_start(GTK_BOX(horizontal), label, TRUE, TRUE, 0);
  // Creando entrybox
  Parametros->entry[0] = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(horizontal), Parametros->entry[0], TRUE, TRUE, 0);
  label2 = gtk_label_new("Password: ");
  gtk_box_pack_start(GTK_BOX(horizontal2), label2, TRUE, TRUE, 0);
  Parametros->entry[1] = gtk_entry_new();
  gtk_entry_set_visibility (GTK_ENTRY (Parametros->entry[1]), FALSE);
  gtk_entry_set_invisible_char (GTK_ENTRY (Parametros->entry[1]), '*');
  // Creando boton
  boton = AddButton(horizontal3, "Ingresar", iniciarSesion);
  gtk_signal_connect(GTK_OBJECT(boton),"clicked",GTK_SIGNAL_FUNC(iniciarSesion), (gpointer)Parametros);
  gtk_box_pack_start(GTK_BOX(horizontal2), Parametros->entry[1], TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vertical), horizontal, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vertical), horizontal2, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vertical), horizontal3, TRUE, TRUE, 0);
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
    fprintf(Archivo, "Neurólogo\n");
    fprintf(Archivo, "Pediatría\n");
    fprintf(Archivo, "1\n");
    fprintf(Archivo, "5545455019\n");
    fprintf(Archivo, "Paseo de la Reforma 880, Lomas de Santa Fe\n");
    fprintf(Archivo, "5514129916\n");
    fprintf(Archivo, "1A\n");
    fprintf(Archivo, "Lunes y Martes\n");
    fprintf(Archivo, "9:00 - 15:00\n");
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
    fscanf(Archivo, " %d", &Nuevo->telefonoUrgencias);
    fscanf(Archivo, " %[^\n]", Nuevo->Direccion);
    fscanf(Archivo, " %d", &Nuevo->telefono);
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
  char id[10];
  Pacientes* Nuevo, *temp;
  FILE* Archivo = fopen("pacientes.txt", "rt");
  if(Archivo == NULL)
    printf("\n");
  else{
    while (fscanf(Archivo, " %[^\n]", id) == 1) {
      Nuevo = (Pacientes*)malloc(sizeof(Pacientes));
      strcpy(Nuevo->id, id);
      fscanf(Archivo, " %[^\n]", Nuevo->Nombre);
      fscanf(Archivo, " %[^\n]", Nuevo->Direccion);
      fscanf(Archivo, " %d", &Nuevo->telefono);
      fscanf(Archivo, " %c", &Nuevo->sexo);
      fscanf(Archivo, " %d", &Nuevo->dia);
      fscanf(Archivo, " %d", &Nuevo->mes);
      fscanf(Archivo, " %d", &Nuevo->anio);
      fscanf(Archivo, " %d", &Nuevo->edad);
      fscanf(Archivo, " %f", &Nuevo->estatura);
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
GtkWidget *AddButton(GtkWidget *theBox, const gchar *buttonText, gpointer CallBackFunction){

    GtkSettings *default_settings = gtk_settings_get_default();
    g_object_set(default_settings, "gtk-button-images", TRUE, NULL);
    GtkWidget *button ;
    button = gtk_button_new_from_stock(GTK_STOCK_OK);
    gtk_box_pack_start(GTK_BOX(theBox),button,FALSE,TRUE, 0);
    gtk_widget_show(button);
    return button;
}
void iniciarSesion(GtkButton *button, gpointer data){
  GtkWidget* dialog;
  Login *datos = (Login*)data;
  const gchar *nombre;
  const gchar *Password;
  Doctores* temp = datos->Lista;
  nombre = gtk_entry_get_text(GTK_ENTRY(datos->entry[0]));
  Password = gtk_entry_get_text(GTK_ENTRY(datos->entry[1]));
  while (temp != NULL && datos->bandera == 0) {
    if(strcmp(temp->FullName, nombre) == 0 && strcmp(temp->Password, Password) == 0)
      datos->bandera = 1;
    temp = temp->sig;
  }
  if(datos->bandera == 1){
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Se ha logeado con éxito");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    gtk_widget_destroy(datos->window);
  }
  else{
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "No ha sido encontrado en la base de datos):");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    gtk_entry_set_text(GTK_ENTRY(datos->entry[0]), "");
    gtk_entry_set_text(GTK_ENTRY(datos->entry[1]), "");
  }
}
