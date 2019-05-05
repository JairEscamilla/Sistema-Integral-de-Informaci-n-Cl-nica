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
  char Doctor[100];
  int bandera;
}Login;
typedef struct _defListas{ // Estructura definida para pasar como parametro las listas deinamicas
  Doctores* ListaDoctores;
  Pacientes* ListaPacientes;
  GtkWidget* entry[15];
  GtkWidget* calendar;
  int sexo, flag;
  char nombreBuscado[200];
  char LoggedDoctor[200];
}ParametrosListas;
typedef struct _defGeneraHistoria{
    GtkWidget* entry[10];
    GtkWidget* window;
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
GtkWidget* menu();
void respuestaMenu(GtkWidget* menu, gpointer data);
void buscar();
void botonesControlA(GtkButton* boton, gpointer data);
void copiarStrings(char campos[11][200]);
void modificarPaciente(const gchar* nombreBuscado, const gchar* nombre, const gchar* direccion, const gchar* telefono, int sexo, const gchar* estatura, const gchar* alergias, const gchar* tipoSangre, const gchar* padecimientosCronicos, int day, int month, int year, Pacientes* ListaPacientes, GtkWidget* fecha, GtkWidget* Ed, int bandera);
void nuevoPaciente(const gchar* nombre, const gchar* direccion,const gchar* telefono, int sexo, const gchar* estatura, const gchar* alergias, const gchar* tipoSangre, const gchar* padecimientosCronicos, int day, int month, int year, Pacientes** ListaPacientes, GtkWidget* fecha, GtkWidget* Ed);
int validarNumeros(const gchar* Cadena, char campo[]);
int validarLetras(const gchar* cadena, char campo[]);
int CalcEdad(int Dia, int Mes, int Anio);
void actualizarArchivoPacientes(Pacientes* ListaPacientes);
void radio(GtkToggleButton* button, gpointer data);
void limpiarCampos(GtkButton *button, gpointer data);
void generarCita(ParametrosListas* datos, const gchar* nombre);
void getDate(char date[]);
void crearCita(GtkWidget* boton, gpointer data);
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
      gtk_box_pack_start(GTK_BOX(theBox),button,TRUE,TRUE, 10);
    }
    if(flag == 4){
      button = gtk_button_new_from_stock(GTK_STOCK_NEW);
      gtk_box_pack_start(GTK_BOX(theBox),button,TRUE,TRUE, 10);
    }
    if(flag == 5){
      button = gtk_button_new_with_label("Mostrar historial médico");
      gtk_box_pack_start(GTK_BOX(theBox),button,TRUE,TRUE, 10);
    }
    if(flag == 6){
      button = gtk_button_new_from_stock(GTK_STOCK_CLEAR);
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
  GtkWidget* window, *menuP, *vertical, *horizontales[11], *label[20], *invisible[11], *boton, *horizontalA, *botonesA[5], *botonLimpiar, *containerDown;
  GtkAdjustment *ajuste;
  char campos[11][200];
  copiarStrings(campos);
  ajuste = GTK_ADJUSTMENT(gtk_adjustment_new(0.0, 0.0, 2.0, 0.2, 2.0, 0));
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
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Sistema de información médica");
  gtk_widget_set_size_request(window, 600, 650);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
  gtk_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(destroy), NULL);
  menuP = menu(); // Creando el menu
  gtk_box_pack_start(GTK_BOX(vertical), menuP, 0, 0, 0);
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
          gtk_box_pack_start(GTK_BOX(horizontales[3]), Listas->entry[i], FALSE, FALSE, 0);
        }else{
          Listas->entry[i] = gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(Listas->entry[3])), "F");
          gtk_box_pack_start(GTK_BOX(horizontales[3]), Listas->entry[i], FALSE, FALSE, 0);
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
      if( i > 4){
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
  strcpy(boton, gtk_button_get_label(button));
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

  if(strcmp("gtk-refresh", boton) == 0)
    modificarPaciente(datos->nombreBuscado, nombre, direccion, telefono, sexo, estatura, alergias, tipoSangre, padecimientosCronicos, day, month, year, datos->ListaPacientes, datos->entry[5], datos->entry[6], datos->flag);
  if(strcmp("gtk-new", boton) == 0){
    nuevoPaciente(nombre, direccion, telefono, sexo, estatura, alergias, tipoSangre, padecimientosCronicos, day, month, year, &datos->ListaPacientes, datos->entry[5], datos->entry[6]);
    datos->nombreBuscado[0] = '\0';
  }
  if(strcmp(boton, "gtk-add") == 0){
    generarCita(datos, nombre);
    return;
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
  temp->sig = NULL;
  if(temp2 == NULL){
    *ListaPacientes = temp;
  }else{
    while (temp2->sig != NULL)
      temp2 = temp2->sig;
    temp2->sig = temp;
  }
  actualizarArchivoPacientes(*ListaPacientes);
  dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Se ha agregado de manera correcta el paciente");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
  gtk_entry_set_text(GTK_ENTRY(fecha), fecnac);
  gtk_entry_set_text(GTK_ENTRY(Ed), edadCaracter);
}
void generarCita(ParametrosListas* datos, const gchar* nombre){
  GtkSettings *default_settings = gtk_settings_get_default();
  g_object_set(default_settings, "gtk-button-images", TRUE, NULL);
  GtkWidget *menuP, *vertical, *label[10], *horizontales[10], *invisible[3], *boton, *titulo;
  GenerarHistoria* Entradas = (GenerarHistoria*)malloc(sizeof(GenerarHistoria));
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
  menuP = menu(); // Creando el menu
  gtk_box_pack_start(GTK_BOX(vertical), menuP, 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(vertical), titulo, 0, 0, 0);
  label[0] = gtk_label_new("Paciente: ");
  label[1] = gtk_label_new("Doctor Tratante: ");
  label[2] = gtk_label_new("Fecha de cita: ");
  label[3] = gtk_label_new("Diagnostico: ");
  label[4] = gtk_label_new("Tratamiento: ");
  label[5] = gtk_label_new("Anotaciones: ");
  for(int i = 0; i < 5; i++){
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
  printf("Generando historia\n");
}
