#include "ami.h"
#include "ami_bmp.h"
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
// funcion que nos permite unir los canales de dos imagenes, separados por una línea negra de
//4 pixeles
void  aan_unir_canales_unsigned_char(unsigned char *canal1,  unsigned char *canal2,
unsigned char **canal_output, int width, int height){
	int i,j,k;
	//asignamos memoria a la nueva imagen
	*canal_output=(unsigned char*)malloc((((2*width)+4)*height)*sizeof(unsigned char));
	//EL indice que cada byte en el vector se calcula en función de la posición que tendria
	//en la matriz
	for (i=0;i<height;i++){
		k=0;
		for(j=0;j<width;j++){
			(*canal_output)[(i*((2*width)+4))+k]=canal1[i*width+j];
			k++;
		};
		for(j=0;j<4;j++){
			(*canal_output)[(i*((2*width)+4))+k]=0;
			k++;

		};
		for(j=0;j<width;j++){
			(*canal_output)[(i*((2*width)+4))+k]=canal2[i*width+j];
			k++;
		};
	};

};

void  aan_unir_canales_float(float *canal1, float *canal2, float **canal_output, int
width, int height) {

	int i,j,k;
	//asignamos memoria a la nueva imagen
	*canal_output=(float*)malloc((((2*width)+4)*height)*sizeof(float));
	//EL indice que cada byte en el vector se calcula en función de la posición que tendria
	//en la matriz
	for (i=0;i<height;i++){
		k=0;
		for(j=0;j<width;j++){
			(*canal_output)[(i*((2*width)+4))+k]=canal1[i*width+j];
			k++;
		};
		for(j=0;j<4;j++){
			(*canal_output)[(i*((2*width)+4))+k]=0;
			k++;

		};
		for(j=0;j<width;j++){
			(*canal_output)[(i*((2*width)+4))+k]=canal2[i*width+j];
			k++;
		};
	};

};


// función que devuelve el valor máximo y mínimo de un canal
void maxmin(unsigned char *canal, int width,int height,int valores[]){
	int i,j;
	valores[0]=(canal)[0];
	valores[1]=(canal)[0];
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			if((canal)[(i*width)+j] < (valores[0])){
				valores[0]=(canal)[(i*width)+j];
			};
			if((canal)[(i*width)+j]>valores[1]){
				valores[1]=(canal)[(i*width)+j];
			};
		};
	};
};

// funcion que normaliza una imagen
void  aan_normalizar_canal_unsigned_char(unsigned char *canal_input, unsigned char
**canal_output, int width, int height){

	int valores[2],i,j;
	float cociente;
	// buscamos el máximo y el mímino de los valores del vector
	maxmin(canal_input,width,height,valores);
	//reservamos memoria para el canal de salida
	*canal_output=(unsigned char*)malloc((width*height)*sizeof(unsigned char));
	cociente= 255/(valores[1]-valores[0]);
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			float mult = ((canal_input)[(i*width)+j])-valores[0];
			//cada posición del nuevo canal tendra el pixel normalizado.
			(*canal_output)[(i*width)+j]=mult*cociente;
		};
	};


};



void   aan_normalizar_imagen_unsigned_char( unsigned char *red, unsigned char
*green, unsigned char *blue, int width, int height) {

	 unsigned char *red_normalizado,*blue_normalizado,*green_normalizado;
	 unsigned char *red_f,*blue_f,*green_f;

	 //normalizamos cada uno de los canales de la imagen
	 aan_normalizar_canal_unsigned_char(red,&red_normalizado,width,height);
	 aan_normalizar_canal_unsigned_char(green,&green_normalizado,width,height);
	 aan_normalizar_canal_unsigned_char(blue,&blue_normalizado,width,height);

	 //unimos lo canales normalizados, con los canales de la imagen original
	aan_unir_canales_unsigned_char(red,red_normalizado,&red_f,width,height);
	aan_unir_canales_unsigned_char(green,green_normalizado,&green_f,width,height);
	aan_unir_canales_unsigned_char(blue,blue_normalizado,&blue_f,width,height);

	char nombre[200];
	printf("\n ¿Con que nombre desea guardar la imagen normalizada?\n");
	scanf("%s",nombre);
	//guardamos la imagen
	ami_write_bmp(nombre,red_f,green_f,blue_f,((width*2)+4),height);

	free(red_normalizado);
	free(green_normalizado);
	free(blue_normalizado);
	free(red_f);
	free(green_f);
	free(blue_f);
};
//funcion que nos devuelve el mayor de 3 valores, e indica en rgb a que
//canal pertenecia.
unsigned char mayor(int a,int b,int c,int *rgb){
	unsigned char valor;

	valor = a;
	*rgb=1;
	if (b> valor){
		valor = b;
		*rgb=2;
	};
	if(c> valor){
		valor = c;
		*rgb=3;
	};
	return valor;
};

//funcion que nos devuelve el menor de tres valores
unsigned char menor(int a,int b,int c){
	unsigned char valor;

	valor = a;
	if (b< valor){
		valor = b;
	};
	if(c< valor){
		valor = c;
	};

	return valor;

}

void extraer_hsv(char *nombre){
	unsigned char *red1,*blue1,*green1;
	unsigned char *V, *H, *S,maximo,minimo;
	int width1,heigth1,i,j,rgb;

	if(ami_read_bmp(nombre,&red1,&green1,&blue1,&width1,&heigth1)== 0){
		    //reservamos memoria para los canales de salida.
			V=(unsigned char*)malloc((width1*heigth1)*sizeof(unsigned char));
			H=(unsigned char*)malloc((width1*heigth1)*sizeof(unsigned char));
			S=(unsigned char*)malloc((width1*heigth1)*sizeof(unsigned char));


			for(i=0;i<heigth1;i++){
				for(j=0;j<width1;j++){

					// Para cada valor de los canales rojo verde azul, calculamos el máximo
					// y mínimo en la posición respectiva y construimos los canales S V H
					// siguiendo las fórmulas dadas
					unsigned char rojo=red1[(i*width1)+j];
					unsigned char verde=green1[(i*width1)+j];
					unsigned char azul=blue1[(i*width1)+j];

					maximo = mayor(rojo,verde,azul,&rgb);
					minimo = menor(rojo,verde,azul);
					V[(i*width1)+j]=maximo;
					S[(i*width1)+j]=maximo-minimo;

					// comprobamos a donde pertenece el máximo y construimos los canales en
					// función de ello
					switch (rgb){
						case 1:{
							float numerador = verde-azul;
							float denominador= maximo-minimo;
							float cociente = numerador / denominador;
							int mult = 43 * cociente;
							H[(i*width1)+j] = mult % 256;
							break;
						};

						case 2:{
							float numerador = azul-rojo;
							float denominador= maximo-minimo;
							float cociente = numerador / denominador;
							float mult = 43 * cociente;
							H[(i*width1)+j] = mult + 85;
							break;
						};

						case 3:{
							float numerador = rojo-verde;
							float denominador= maximo-minimo;
							float cociente = numerador / denominador;
							float mult = 43 * cociente;
							H[(i*width1)+j] = mult + 170;
							break;
						};

					};

				};
			};
			char*nombre2;
			unsigned char *red,*blue,*green;
			// unimos los 3 canales de las imagenes
			nombre2 = malloc(255 * sizeof(char));
			aan_unir_canales_unsigned_char(red1,H,&red,width1,heigth1);
			aan_unir_canales_unsigned_char(green1,H,&green,width1,heigth1);
			aan_unir_canales_unsigned_char(blue1,H,&blue,width1,heigth1);
			// guardamos la nueva imagen
			strcpy(nombre2,nombre);
			ami_write_bmp(strncat(nombre2,"_H.bmp",6),red,green,blue,((width1*2)+4),heigth1);

			aan_unir_canales_unsigned_char(red1,V,&red,width1,heigth1);
			aan_unir_canales_unsigned_char(green1,V,&green,width1,heigth1);
			aan_unir_canales_unsigned_char(blue1,V,&blue,width1,heigth1);
			// guardamos la nueva imagen
			strcpy(nombre2,nombre);
			ami_write_bmp(strncat(nombre2,"_V.bmp",6),red,green,blue,((width1*2)+4),heigth1);

			aan_unir_canales_unsigned_char(red1,S,&red,width1,heigth1);
			aan_unir_canales_unsigned_char(green1,S,&green,width1,heigth1);
			aan_unir_canales_unsigned_char(blue1,S,&blue,width1,heigth1);
			// guardamos la nueva imagen
			strcpy(nombre2,nombre);
			ami_write_bmp(strncat(nombre2,"_S.bmp",6),red,green,blue,((width1*2)+4),heigth1);

			// Liberamos la memoria usada
			free(red);
			free(green);
			free(blue);
			free(H);
			free(S);
			free(V);


		}
	else{
		printf("\n lo sentimos, no hemos podido abrir la imagen indicada\n");
	};


};


// Menu principal //////////////////////////////////////////////

int main(){
	int opcion;
	printf("\nPor favor indique que operación desea realizar:\n\n   1 Unir dos imágenes\n   2 Normalizar una imagen\n   3 Obtener canales HSV de una imagen\n 4 Unir dos imágenes con float\n\n");
	scanf("%d",&opcion);
	switch(opcion){
		// opcion para unir 2 imagenes
		case 1:{
			char fichero1[200],fichero2[200],ficherosalida[200];
			unsigned char *red1,*blue1,*green1,*red2,*blue2,*green2;
			int width1,heigth1,width2,heigth2;

			printf("\n Inserte el  directorio de la primera imagen\n\n");
			scanf("%s",fichero1);
			printf("\n Inserte el  directorio de la segunda imagen\n\n");
			scanf("%s",fichero2);
			printf("\n Inserte el  directorio donde desea guardar la imagen obtenida\n\n");
			scanf("%s",ficherosalida);

			//Comprobamos que las imagenes se habran correctamente
			if(ami_read_bmp(fichero1,&red1,&green1,&blue1,&width1,&heigth1)== 0){
				if(ami_read_bmp(fichero2,&red2,&green2,&blue2,&width2,&heigth2)== 0){
					unsigned char *red,*blue,*green;
					// unimos los 3 canales de las imagenes
					aan_unir_canales_unsigned_char(red1,red2,&red,width1,heigth1);
					aan_unir_canales_unsigned_char(green1,green2,&green,width1,heigth1);
					aan_unir_canales_unsigned_char(blue1,blue2,&blue,width1,heigth1);

					// guardamos la nueva imagen
					ami_write_bmp(ficherosalida,red,green,blue,((width1*2)+4),heigth1);
					free(red);
					free(green);
					free(blue);
				}
				else printf("\n el directorio de la segunda imagen es incorrecto\n");
			}
			else{
				printf("\n el directorio de la primera imagen es incorrecto\n");
			};
			break;
		};

		// opción para normalizar una imagen
		case 2:{
			char fichero1[200];
			unsigned char *red1,*blue1,*green1;
			int width1,heigth1;

			printf("\n Inserte el  directorio de la imagen a normalizar\n\n");
			scanf("%s",fichero1);

			if(ami_read_bmp(fichero1,&red1,&green1,&blue1,&width1,&heigth1)== 0){
				//invocamos a la función que normaliza imagenes
				aan_normalizar_imagen_unsigned_char(red1,green1,blue1,width1,heigth1);

			}
			else{
				printf("\n lo sentimos, no hemos podido abrir la imagen indicada\n");
			};
			break;
		};
		// opcion para obtener los canales hsv
		case 3:{
			char fichero1[200];
						printf("\n Inserte el  directorio de la imagen a tratar\n\n");
						scanf("%s",fichero1);
						extraer_hsv(fichero1);
						break;
		};
		case 4:{
			char fichero1[200],fichero2[200],ficherosalida[200];
			unsigned char *red1,*blue1,*green1,*red2,*blue2,*green2;
			int width1,heigth1,width2,heigth2, i;

			printf("\n Inserte el  directorio de la primera imagen\n\n");
			scanf("%s",fichero1);
			printf("\n Inserte el  directorio de la segunda imagen\n\n");
			scanf("%s",fichero2);
			printf("\n Inserte el  directorio donde desea guardar la imagen obtenida\n\n");
			scanf("%s",ficherosalida);

			//Comprobamos que las imagenes se habran correctamente
			if(ami_read_bmp(fichero1,&red1,&green1,&blue1,&width1,&heigth1)== 0){
				if(ami_read_bmp(fichero2,&red2,&green2,&blue2,&width2,&heigth2)== 0){
					unsigned char *red,*blue,*green;
					// unimos los 3 canales de las imagenes
					//
					float *float_red, *float_blue, *float_green,*float_red1,*float_red2,
					      *float_blue1,*float_blue2,*float_green1,*float_green2;
					float_red1 = malloc(sizeof(float)*width1*heigth1 ); //ojito, hay q liberar dp
					float_red2 = malloc(sizeof(float)*width2*heigth2 ); //ojito, hay q liberar dp 
					float_blue1 = malloc(sizeof(float)*width1*heigth1 ); //ojito, hay q liberar dp 
					float_blue2 = malloc(sizeof(float)*width2*heigth2 ); //ojito, hay q liberar dp 
					float_green1 = malloc(sizeof(float)*width1*heigth1 ); //ojito, hay q liberar dp 
					for (i=0;i<width1*heigth1; i++)
					{
						float_red1[i]=red1[i];
						float_blue1[i]=blue1[i];
						float_green1[i]=green1[i];
					}
					float_green2 = malloc(sizeof(float)*width2*heigth2 ); //ojito, hay q liberar dp 
					for (i=0;i<width2*heigth2; i++)
					{
						float_red2[i]=red2[i];
						float_blue2[i]=blue2[i];
						float_green2[i]=green2[i];
					}

					aan_unir_canales_float(float_red1,float_red2,&float_red,width1,heigth1);
					aan_unir_canales_float(float_green1,float_green2,&float_green,width1,heigth1);
					aan_unir_canales_float(float_blue1,float_blue2,&float_blue,width1,heigth1);
					for (i=0;i<(width1*2 + 4) * heigth1; i++)
					{
						red[i]=(unsigned char)float_red[i];
						blue[i]=(unsigned char)float_blue[i];
						green[i]=(unsigned char)float_green[i];
					}
					
					// guardamos la nueva imagen
					ami_write_bmp(ficherosalida,red,green,blue,((width1*2)+4),heigth1);
					free(float_red); 
					free(float_blue); 
					free(float_green);
					free(float_red1);
					free(float_red2);
					free(float_blue1);
					free(float_blue2);
					free(float_green1);
					free(float_green2);
					free(red);
					free(green);
					free(blue);
				}
				else printf("\n el directorio de la segunda imagen es incorrecto\n");
			}
			else{
				printf("\n el directorio de la primera imagen es incorrecto\n");
			};
			break;
		};
	};
return 0;
};
