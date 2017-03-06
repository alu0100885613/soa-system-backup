# Backup distribuido
Con la ayuda de Qt vamos a crear nuestro propio sistema de backup distribuido.

En principio la aquitectura será muchos a uno. Es decir, un cliente enviará los datos a un servidor. Este los distribuirá a varios clientes, que a su vez almacenarán los archivos en su sistema de archivos local:

 * Necesitaremos dos programas: un servidor y un cliente de backup.
 * La función del servidor es recibir los datos desde el cliente de origen y retransmitirlos hacia los clientes de destino.
   * Sólo hace falta que haya un servidor en ejecución.
   * El puerto de escucha del servidor debe ser configurable desde la GUI.
 * Al mismo tiempo se pueden estar ejecutándo múltiples clientes en distintas máquinas.
   * Uno será el origen de los datos a copiar. Los leerá y enviará al servidor para su retransmisión.
   * El resto recibirán los datos desde el servidor para hacer la copia de seguridad en un directorio local.
   * La IP y el puerto del servidor al que deben conectarse los clientes debe ser configurable desde la GUI
   * En cada cliente la carpeta origen o destino de los archivos deben ser configurable desde la GUI.
 * El sistema podría funcionar a así:
   1. El servidor se inicia. El usuario puede configurar el puerto y ponerlo a la escucha.
   2. Un cliente se inicia.
      * El usuario puede configurar la dirección del servidor, elegir la carpeta de destino y conectarlo como cliente de destino.
      * O, el usuario puede configurar la dirección del servidor, elegir la carpeta de origen, indicar cuántos destinos deben estar conectados y conectar como cliente de origen.
   3. Los clientes de destino esperan indefinidamente hasta que el servidor inicia la transferencia.
   4. Los clientes de origen esperan hasta que hay suficientes clientes conectados en el servidor, entonces empiezan a recorrer el directorio y a transferir hace el servidor.
   5. El servidor envía los datos recibidos del cliente origen a los clientes destino, que deben reconstruir el arbol de directorios en su carpeta local.
   6. Durante la transferencia los clientes y el servidor deben mostrar el progreso de la copia:
      * El nombre del archivo que se está copiando actualmente.
      * Una barra de progreso, que puede ser global de toda la copia o sólo del progreso sobre el archivo actual.
   7. Hay que incluir medidas de contrapresión para evitar problemas como los mencionados en la teoría.

## Cómo empezar

 1. Acepta la [tarea asignada de GitHub Classroom](https://classroom.github.com/assignment-invitations/e00c36345e701f3f266fbc98b2a979a5). Así obtendrás tu propio repositorio como una copia de este. A este repositorio lo llamaremos `upstream`.
 2. Haz un [fork](https://guides.github.com/activities/forking/) de `upstream`. Al nuevo repositorio lo llamaremos `origin`.
 3. [Clona](http://gitref.org/creating/#clone) `origin` en tu ordenador.
 4. Trabaja en tu copia local para desarrollar tu monitor del sistema, siguiendo los pasos indicados en el siguiente apartado.
 5. Modifica README.md usando [Markdown](https://guides.github.com/features/mastering-markdown/) para:
   1. Explicar cómo compilar y probar la aplicación, incluyendo los requisitos requeridos para hacerlo.
   2. Explicar los detalles del diseño del protocolo utilizado. Imagina que lo estás explicando para que otro desarrollador pueda crear un cliente o un servidor compatible.
   3. Comentar las características implementadas.
 5. [Sube](http://gitref.org/remotes/#push) los cambios al repositorio `origin` en GitHub.
 6. Crea un [pull request](https://help.github.com/articles/creating-a-pull-request) de `origin` a `upstream` para entregar la práctica.

## Protocolo

El paso más importante es el diseño del protocolo. Algunas cuestiones son generales:

 * ¿Qué protocolo de transporte usar? TCP, UDP, etc.
 * ¿Protocolo binario o texto?
 * ¿Usar un formato conocido...? [JSON](http://doc.qt.io/qt-5/json.html), [Protocol Buffer](https://jmtorres.webs.ull.es/me/2013/03/implementando-un-protocolo-con-protocol-buffers/), etc.
 * ¿...o crear nuestro propio formato? ¿con delimitadores? ¿tamaño fijo? ¿inspirarnos en un protocolo conocido, como HTTP?
 * ¿Cómo vamos a comprobar que la conexión sigue activa.

Otras dependen de la aplicación concreta que estamos a desarrollando. Hay que pensar en las distintas etapas y qué información hay que comunicar y a quién en cada una:

 * ¿Serán necesarios diferentes tipos de mensajes? ¿cómo se van a diferenciar unos de otros?
 * ¿Cómo va a saber el cliente origen cuando iniciar la transferencia?
 * ¿Qué información debe ir junto a los datos de los archivos? ¿ruta? ¿tamaño?
 * ¿Qué información hace falta para mantener actualizada las barras de progreso?
 
Puedes elegir la soluciones que prefieras.

## Opcional

 * No sería muy complicado tener un sistema muchos a muchos. Muchos clientes origen mandando sus copias a un servidor para que los múltiples clientes destino hagan copia. En ese caso sería conveniente que en los paquetes se incluyera el nombre de la máquina origen, para organizar los archivos en distintos subdirectorios, según el cliente que los envía, evitando colisiones.
