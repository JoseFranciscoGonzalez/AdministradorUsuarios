# AdministradorUsuarios
AdministradorUsuarios es un programa de lectura, escritura y búsqueda de información de usuarios contenida en archivos de configuración.txt implementado en ANSI-C89 y de modo consola.

## Compilar
1. Clonar este repositorio
2. En la carpeta, abrir la terminal y ejecutar $ make.

## Como usar
- Visualizar usuarios: ./social_network --eliminate 0 --output single TEST_file.txt TEST_file2.txt
- Aislar archivos de usuario: ./social_network --eliminate 0 --output multi TEST_file.txt
- Eliminar usuario por id: ./social_network --eliminate 1000 --output single TEST_file.txt
- Eliminar usuario por nombre: ./social_network --eliminate u: Mariano --output single TEST_file.txt
