GtkWidget *ingresar = gtk_image_new_from_file ("Iconos/Ingresar.png");
GtkWidget *buscar = gtk_image_new_from_file ("Iconos/Buscar.png");
GtkWidget *actualizar = gtk_image_new_from_file ("Iconos/Actualizar.png");
GtkWidget *nuevacita = gtk_image_new_from_file ("Iconos/NuevaCita.png");
GtkWidget *nuevopaciente = gtk_image_new_from_file ("Iconos/NuevoPaciente.png");
GtkWidget *historial = gtk_image_new_from_file ("Iconos/Historial.png");
GtkWidget *vaciar = gtk_image_new_from_file ("Iconos/Vaciar.png");

if(flag == 0){
  button = gtk_button_new();
  gtk_button_set_image (GTK_BUTTON (button), ingresar);
  gtk_button_set_relief (GTK_BUTTON (button), GTK_RELIEF_NONE);
  gtk_box_pack_start(GTK_BOX(theBox),button,TRUE,TRUE, 0);
 }
if(flag == 1){
  button = gtk_button_new();
  gtk_button_set_image (GTK_BUTTON (button), buscar);
  gtk_button_set_relief (GTK_BUTTON (button), GTK_RELIEF_NONE);
  gtk_box_pack_start(GTK_BOX(theBox),button,TRUE,TRUE, 15);
 }
if(flag == 2){
  button = gtk_button_new();
  gtk_button_set_image (GTK_BUTTON (button), actualizar);
  gtk_button_set_relief (GTK_BUTTON (button), GTK_RELIEF_NONE);
  gtk_box_pack_start(GTK_BOX(theBox),button,TRUE,TRUE, 15);
 }
if(flag == 3){
  button = gtk_button_new();
  gtk_button_set_image (GTK_BUTTON (button), nuevacita);
  gtk_button_set_relief (GTK_BUTTON (button), GTK_RELIEF_NONE);
  gtk_box_pack_start(GTK_BOX(theBox),button,TRUE,TRUE, 10);
 }
if(flag == 4){
  button = gtk_button_new();
  gtk_button_set_image (GTK_BUTTON (button), nuevopaciente);
  gtk_button_set_relief (GTK_BUTTON (button), GTK_RELIEF_NONE);
  gtk_box_pack_start(GTK_BOX(theBox),button,TRUE,TRUE, 10);
 }
if(flag == 5){
  button = gtk_button_new();
  gtk_button_set_image (GTK_BUTTON (button), historial);
  gtk_button_set_relief (GTK_BUTTON (button), GTK_RELIEF_NONE);
  gtk_box_pack_start(GTK_BOX(theBox),button,TRUE,TRUE, 10);
 }
if(flag == 6){
  button = gtk_button_new();
  gtk_button_set_image (GTK_BUTTON (button), vaciar);
  gtk_button_set_relief (GTK_BUTTON (button), GTK_RELIEF_NONE);
  gtk_box_pack_start(GTK_BOX(theBox),button,TRUE,TRUE, 15);
 }
