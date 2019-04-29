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
  int bandera;
}Login;
typedef struct _defListas{ // Estructura definida para pasar como parametro las listas deinamicas
  Doctores* ListaDoctores;
  Pacientes* ListaPacientes;
  GtkWidget* entry[10];
  GtkWidget* calendar;
}ParametrosListas;
// Prototipos de las funciones
void leerListaDoctores(Doctores**);
void leerListaPacientes(Pacientes**);
void leerHistorial(Pacientes*);
void destroy(GtkWidget* wideget, gpointer data);
GtkWidget *AddButton(GtkWidget *theBox, const gchar *buttonText, gpointer CallBackFunction, int flag);
void iniciarSesion(GtkButton *button, gpointer data);
void loger(Doctores*, Login*, int*);
void entrandoSistema(ParametrosListas*);
GtkWidget* menu();
void respuestaMenu(GtkWidget* menu, gpointer data);
void buscar();
void botonesControlA(GtkButton* boton, gpointer data);
void copiarStrings(char campos[11][200]);
void modificarPaciente(const gchar* nombre, const gchar* direccion, const gchar* telefono, const gchar* sexo, const gchar* estatura, const gchar* alergias, const gchar* tipoSangre, const gchar* padecimientosCronicos, int day, int month, int year, Pacientes* ListaPacientes, GtkWidget* fecha, GtkWidget* Ed);
int validarNumeros(const gchar* Cadena, char campo[]);
int validarLetras(const gchar* cadena, char campo[]);
int CalcEdad(int Dia, int Mes, int Anio);
void actualizarArchivoPacientes(Pacientes* ListaPacientes);
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
  loger(ListaDoctores, Parametros, &flag); // Despliega la ventana de inicio de sesion
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
  boton = AddButton(horizontal3, "Ingresar", iniciarSesion, 0);
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
    GtkWidget *button;
    if(flag == 0){
      button = gtk_button_new_from_stock(GTK_STOCK_OK);
      gtk_box_pack_start(GTK_BOX(theBox),button,TRUE,TRUE, 0);
    }
    if(flag == 1){
      button = gtk_button_new_from_stock(GTK_STOCK_FIND);
      gtk_box_pack_start(GTK_BOX(theBox),button,TRUE,TRUE, 15);
    }
    if(flag == 2){
      button = gtk_button_new_from_stock(GTK_STOCK_REFRESH);
      gtk_box_pack_start(GTK_BOX(theBox),button,TRUE,TRUE, 15);
    }
    if(flag == 3){
      button = gtk_button_new_from_stock(GTK_STOCK_ADD);
      gtk_box_pack_start(GTK_BOX(theBox),button,TRUE,TRUE, 15);
    }
    if(flag == 4){
      button = gtk_button_new_from_stock(GTK_STOCK_NEW);
      gtk_box_pack_start(GTK_BOX(theBox),button,TRUE,TRUE, 15);
    }
    if(flag == 5){
      button = gtk_button_new_with_label("Mostrar historial médico");
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
// Funcion que muestra la ventana principal del sistema
void entrandoSistema(ParametrosListas* Listas){
  GtkWidget* window, *menuP, *vertical, *horizontales[10], *label[12], *invisible[11], *boton, *horizontalA, *botonesA[4];
  char campos[11][200];
  copiarStrings(campos);
  // Creando las cajas
  vertical = gtk_vbox_new(0, 0);
  for(int i = 0; i < 11; i++){
    horizontales[i] = gtk_hbox_new(TRUE, 2);
  }
  horizontalA = gtk_hbox_new(TRUE, 2);
  // Creando ventana principal
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Sistema de información médica");
  gtk_widget_set_size_request(window, 600, 600);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
  gtk_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(destroy), NULL);
  menuP = menu(); // Creando el menu
  gtk_box_pack_start(GTK_BOX(vertical), menuP, 0, 0, 0);
  // Elementos principales de la interfaz
  for(int i = 0; i < 11; i++){
    if(i < 10){
      label[i] = gtk_label_new(campos[i]);
      gtk_box_pack_start(GTK_BOX(horizontales[i]), label[i], TRUE, TRUE, 0);
      Listas->entry[i] = gtk_entry_new();
      gtk_box_pack_start(GTK_BOX(horizontales[i]), Listas->entry[i], TRUE, TRUE, 0);

      if(i == 5 || i == 4){
        gtk_entry_set_editable(GTK_ENTRY(Listas->entry[i]), FALSE);
      }
      if(i != 0){
        invisible[i] = gtk_label_new(NULL);
        gtk_box_pack_start(GTK_BOX(horizontales[i]), invisible[i], TRUE, TRUE, 0);
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
  // Creando botones de abajo
  for(int i = 0; i < 4; i++){
    botonesA[i] = AddButton(horizontalA, "Actualizar", botonesControlA, i+2);
    gtk_signal_connect(GTK_OBJECT(botonesA[i]), "clicked", GTK_SIGNAL_FUNC(botonesControlA), (gpointer)Listas);
  }
  invisible[10] = gtk_label_new(NULL);
  gtk_box_pack_start(GTK_BOX(vertical), horizontalA, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vertical), invisible[10], TRUE, TRUE, 0);
  gtk_container_add(GTK_CONTAINER(window), vertical);
  gtk_widget_show_all(window);
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
GtkWidget* menu(){
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
    gtk_signal_connect(GTK_OBJECT(doctoresitem), "activate", GTK_SIGNAL_FUNC(respuestaMenu), NULL);
    doctoresitem = gtk_menu_item_new_with_label("Listado de doctores");
    gtk_menu_shell_append(GTK_MENU_SHELL(docs), doctoresitem);
    gtk_signal_connect(GTK_OBJECT(doctoresitem), "activate", GTK_SIGNAL_FUNC(respuestaMenu), NULL);
    doctoresitem = gtk_menu_item_new_with_label("Desplegar doctores por especialidad");
    gtk_menu_shell_append(GTK_MENU_SHELL(docs), doctoresitem);
    gtk_signal_connect(GTK_OBJECT(doctoresitem), "activate", GTK_SIGNAL_FUNC(respuestaMenu), NULL);
  // Creando los submenus para pacientes
    pacientesitem = gtk_menu_item_new_with_label("Desplegar lista de pacientes");
    gtk_menu_shell_append(GTK_MENU_SHELL(pacientes), pacientesitem);
    gtk_signal_connect(GTK_OBJECT(pacientesitem), "activate", GTK_SIGNAL_FUNC(respuestaMenu), NULL);
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
  int flag = 0;
  ParametrosListas* datos = (ParametrosListas*)data;
  Pacientes* temp = datos->ListaPacientes;
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
    }else{
      dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Se ha encontrado con éxito al paciente buscado");
      gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
      gtk_entry_set_text(GTK_ENTRY(datos->entry[0]), temp->Nombre);
      gtk_entry_set_text(GTK_ENTRY(datos->entry[1]), temp->Direccion);
      gtk_entry_set_text(GTK_ENTRY(datos->entry[2]), temp->telefono);
      gtk_entry_set_text(GTK_ENTRY(datos->entry[3]), temp->sexo);
      gtk_entry_set_text(GTK_ENTRY(datos->entry[4]), temp->fecnac);
      gtk_entry_set_text(GTK_ENTRY(datos->entry[5]), temp->edad);
      gtk_entry_set_text(GTK_ENTRY(datos->entry[6]), temp->estatura);
      gtk_entry_set_text(GTK_ENTRY(datos->entry[7]), temp->alergias);
      gtk_entry_set_text(GTK_ENTRY(datos->entry[8]), temp->tipoSangre);
      gtk_entry_set_text(GTK_ENTRY(datos->entry[9]), temp->PadecimientosCronicos);
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
  strcpy(campos[5], "Edad: ");
  strcpy(campos[6], "Estatura: ");
  strcpy(campos[7], "Alergias: ");
  strcpy(campos[8], "Tipo de sangre: ");
  strcpy(campos[9], "Padecimientos crónicos");
}
// Botones de control de la parte de abajo
void botonesControlA(GtkButton *button, gpointer data){
  ParametrosListas* datos = (ParametrosListas*)data;
  int validacion[7];
  char boton[200];
  const gchar* nombre, *direccion, *telefono, *sexo, *estatura, *alergias, *tipoSangre, *padecimientosCronicos;
  guint year, month, day;
  nombre = gtk_entry_get_text(GTK_ENTRY(datos->entry[0]));
  direccion = gtk_entry_get_text(GTK_ENTRY(datos->entry[1]));
  telefono = gtk_entry_get_text(GTK_ENTRY(datos->entry[2]));
  sexo = gtk_entry_get_text(GTK_ENTRY(datos->entry[3]));
  estatura = gtk_entry_get_text(GTK_ENTRY(datos->entry[6]));
  alergias = gtk_entry_get_text(GTK_ENTRY(datos->entry[7]));
  tipoSangre = gtk_entry_get_text(GTK_ENTRY(datos->entry[8]));
  padecimientosCronicos = gtk_entry_get_text(GTK_ENTRY(datos->entry[9]));
  gtk_calendar_get_date(GTK_CALENDAR(datos->calendar), &year, &month, &day);
  strcpy(boton, gtk_button_get_label(button));
  validacion[0] = validarLetras(nombre, "Nombre paciente");
  validacion[1] = validarNumeros(telefono, "Telefono");
  validacion[2] = validarLetras(sexo, "Sexo");
  validacion[3] = validarNumeros(estatura, "Estatura");
  validacion[4] = validarLetras(alergias, "Alergias");
  validacion[5] = validarLetras(tipoSangre, "Tipo de sangre");
  validacion[6] = validarLetras(padecimientosCronicos, "PadecimientosCronicos");
  for(int i = 0; i < 8; i++)
    if(validacion[i] == 1)
      return;
  month++;

  if(strcmp("gtk-refresh", boton) == 0)
    modificarPaciente(nombre, direccion, telefono, sexo, estatura, alergias, tipoSangre, padecimientosCronicos, day, month, year, datos->ListaPacientes, datos->entry[4], datos->entry[5]);
}
void modificarPaciente(const gchar* nombre, const gchar* direccion, const gchar* telefono, const gchar* sexo, const gchar* estatura, const gchar* alergias, const gchar* tipoSangre, const gchar* padecimientosCronicos, int day, int month, int year, Pacientes* ListaPacientes, GtkWidget* fecha, GtkWidget* Ed){
  Pacientes* temp = ListaPacientes;
  GtkWidget* dialog;
  char fecnac[200], edadCaracter[10];
  int edad;
  while (temp != NULL && strcmp(nombre, temp->Nombre) != 0) {
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
  strcpy(temp->sexo, sexo);
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
