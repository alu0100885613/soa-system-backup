# Backup distribuido
Sistema de backup distribuido utilizando Qt.

Características del sistema desarrollado:

    * Existen 3 tipos de modos para el programa:
        - Modo `Active User` (Los que envían los archivos, puede haber más de uno al mismo tiempo conectado)
        - Modo `Passive User` (Los que reciben los archivos y los alojan, puede haber más de uno al mismo tiempo)
        - Modo `Server` (El que tiene contacto con todos los clientes y reenvía los archivos, 1 por puerto)


    * Los usuarios activos solo podrán transmitir cuando exista 1 usuario pasivo conectado al mismo servidor. Solo podrá transmitir un usuario activo al mismo tiempo, los otros si lo intentan recibirán un mensaje de canal ocupado.
    * No se permitirá realizar conexiones sin haber indicado previamente el modo y el directorio.
    * Los campos y botones que un modo no se utilizen, serán bloqueados.
    * Existirá un timer que indicará el tiempo desde que se pulsó el botón de Connect.
    * Existirá un comboBox que indicará el número de determinados tipos de usuarios conectados. (Los activos verán a los pasivos y los pasivos a los activos, y el Server a ambos)
    * Existirá una barra de progreso
    * Existirán dos labels, uno encima de otro, el de encima es la ip de la interfaz de red del propio usuario y el de debajo es el puerto al se que está conectado.
    * Se pueden enviar árboles de directorios.


## Compilación y Requisitos

 1. Se deberá tener instalada la libería protobuf y referenciarla desde el .pro.
 2. En las Settings del proyecto se debe desactivar el Shadow Build
 3. En la carpeta del proyecto en consola habrá que generar los archivos de protobuf ejecutando `protoc 
-cpp_out. protocolbuffer.proto`
 4. Ejecutar el comando `qmake`
 5. Ejecutar `make`

## Protocolo

 * Protocolo de transporte TCP.
 * Protocolo de texto.
 * Protocolo: [Protocol Buffer](https://jmtorres.webs.ull.es/me/2013/03/implementando-un-protocolo-con-protocol-buffers/)
 * Envío de paquete KeepAlive cada 15 segundos, activando un timer que decida el tiempo en el que puede llegar el ACK antes de tomar a algún cliente por inactivo.
 * Diferenciación de mensajes por medio de un campo type_ en la trama.
 * El servidor informará a todos sobre, el número de usuarios de cada tipo cuando se produzca una actualización en este contador. Internamente los usuarios activos activarán su botón de Send cuando sean avisados que hay algún usuario pasivo conectado.
 * Antes de la transferencia de archivo se enviará el tamaño total de archivos, su ruta y tamaño individual.
 * La barra de progreso se calculará mediante el tamaño total de todos los archivos y lo que actualmente se ha enviado/escrito.
 * Con cada envío de directorio o archivo, se indicará de que tipo es y todos los pasivos responderán con paquetes de confirmación para esperar que todos hayan recibido correctamente, antes de enviar el siguiente archivo o directorio.
 * Recorriendo el directorio a enviar se recorrerá e irán enviando los directorios, mientras que los datos de los ficheros se meterán en una cola para su posterior extracción.
 * El servidor mantendrá una lista de los socket que están en fase de transferencia por si a mitad de una se uno o se va algún usuario.
 * Antes de cada desconexión el servidor será informado e informará al resto de usuarios, inclusive si es él el que se desconecta, en este caso forzará a todos los clientes a terminar su conexión.


Eduardo Borges Fernández. ULL.
