all: 
	gcc -o demo main.c -I include -lGLEW -lglfw -lGL -lX11 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lXinerama -lXcursor -ldl -lrt -lm
