# Backup distribuido
Con la ayuda de Qt vamos a crear nuestro propio sistema de backup distribuido, que tendrá las siguientes características:

 * Necesitaremos dos programas: un servidor y un cliente de backup.
 * La función del servidor es recibir los datos desde el cliente de origen y retransmitirlos hacia los clientes de destino.
   * Sólo hace falta que haya un servidor en ejecución.
   * El puerto de escucha del servidor debe ser configurable.
 * Al mismo tiempo se pueden estar ejecutándo múltiples clientes en distintas máquinas.
   * Uno será el origen de los datos a copiar. Los leerá y enviará al servidor para su retransmisión.
   * El resto recibirán los datos desde el servidor para hacer la copia de seguridad en un directorio local.
   * La IP y el puerto del servidor al que deben conectarse los clientes debe ser configurable.
   * En cada cliente la carpeta origen o destino de los archivos deben ser configurable.
 * El sistema podría funcionar a así:
   1. El servidor se inicia. El usuario puede configurar el puerto y ponerlo a la escucha
   2. Un cliente se inicia.
      1. El usuario puede configurar la dirección del servidor, elegir la carpeta de destino y conectarlo como cliente de destino.
      2. O, el usuario puede configurar la dirección del servidor, elegir la carpeta de origen, cuantos destinos necesita y conectarlo como cliente de origen.
   3. Los clientes de destino esperan indefinidamente hasta que el servidor inicia la transferencia.
   4. Los clientes de origen esperan hasta que hay suficientes clientes conectados en el servidor, entonces empiezan a recorrer el directorio y a transferir hace el servidor.
   5. El servidor envía los datos recibidos del cliente origen a los clientes destino, que deben reconstruir el directorio en su carpeta local.
   6. Durante la transferencia los clientes y el servidor deben mostrar el progreso de la copia:
      * El nombre del archivo que se está copiando actualmente.
      * Una barra de progreso, que puede ser global de toda la copia o sólo del progreso sobre el archivo actual.

## Cómo empezar

 1. Acepta la [tarea asignada de GitHub Classroom](https://classroom.github.com/assignment-invitations/e00c36345e701f3f266fbc98b2a979a5). Así obtendrás tu propio repositorio como una copia de este. A este repositorio lo llamaremos `upstream`.
 2. Haz un [fork](https://guides.github.com/activities/forking/) de `upstream`. Al nuevo repositorio lo llamaremos `origin`.
 3. [Clona](http://gitref.org/creating/#clone) `origin` en tu ordenador.
 4. Trabaja en tu copia local para desarrollar tu monitor del sistema, siguiendo los pasos indicados en el siguiente apartado.
 5. Modifica README.md usando [Markdown](https://guides.github.com/features/mastering-markdown/) para:
   1. Explicar cómo compilar y probar la aplicación, incluyendo los requisitos requeridos para hacerlo.
   2. Comentar las características implementadas.
   3. Discute que técnica para manejar la concurrencia te pareció mejor: más sencilla, más flexible o más escalable.
 5. [Sube](http://gitref.org/remotes/#push) los cambios al repositorio `origin` en GitHub.
 6. Crea un [pull request](https://help.github.com/articles/creating-a-pull-request) de `origin` a `upstream` para entregar la práctica.
