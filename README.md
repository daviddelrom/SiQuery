### Guía de Instalación y Configuración para la Base de Datos

Sigue los pasos a continuación para instalar y configurar MySQL y las herramientas necesarias en tu sistema para gestionar el proyecto de universidades, solicitudes SICUE y becas.

#### 1. **Instalación del Motor de Base de Datos**

1. **Instalar MySQL en Ubuntu (Linux)**:
    - Actualiza los repositorios e instala MySQL:
      ```bash
      sudo apt update
      sudo apt install mysql-server
      ```
    - Configura la instalación de MySQL:
      ```bash
      sudo mysql_secure_installation
      ```
    - Inicia el servicio de MySQL:
      ```bash
      sudo service mysql start
      ```

#### 2. **Conexión a MySQL**

1. Conéctate a MySQL con el usuario `root`:
    ```bash
    sudo mysql -u root -p
    ```

#### 3. **Creación de la Base de Datos y Tablas**

1. Crea la base de datos:
    ```sql
    CREATE DATABASE IS;
    USE IS;
    ```
2. Ejecuta los comandos SQL para crear las tablas necesarias (en el directorio /src/DB).

#### 4. **Configuración del Procedimiento Almacenado**

1. Crea el procedimiento almacenado para actualizar las plazas ocupadas:
    ```sql
    DELIMITER $$

    CREATE PROCEDURE ActualizarPlazasOcupadas(IN solicitud_id INT, IN tipo_solicitante ENUM('Estudiante', 'Profesor'))
    BEGIN
        DECLARE universidad_destino_id INT;
        
        -- Obtener el ID de la universidad de destino
        SELECT universidad_destino INTO universidad_destino_id FROM Solicitud WHERE solicitud_id = solicitud_id;
        
        -- Actualizar el número de plazas ocupadas
        IF tipo_solicitante = 'Estudiante' THEN
            UPDATE Universidad 
            SET numero_ocupados_alumnos = numero_ocupados_alumnos + 1 
            WHERE universidad_id = universidad_destino_id;
        ELSEIF tipo_solicitante = 'Profesor' THEN
            UPDATE Universidad 
            SET numero_ocupados_profesores = numero_ocupados_profesores + 1 
            WHERE universidad_id = universidad_destino_id;
        END IF;
    END $$

    DELIMITER ;
    ```

#### 5. **Inserción de Datos Ejemplo**

1. Inserta universidades:
    ```sql
    INSERT INTO Universidad (nombre, direccion, ciudad, numero_maximo, maximo_alumnos, maximo_profesores) VALUES
    ('Universidad A', 'Calle 123, Ciudad A', 'Ciudad A', 100, 50, 10),
    ('Universidad B', 'Calle 456, Ciudad B', 'Ciudad B', 150, 70, 20);
    ```
2. Inserta usuarios, alumnos, profesores, solicitudes, SICUE y becas siguiendo los comandos en el script.

#### 6. **Comprobación de Datos**

1. Ejecuta las siguientes consultas para comprobar que los datos fueron insertados correctamente:
    ```sql
    SELECT * FROM Solicitud;
    SELECT * FROM SolicitudSicue;
    SELECT * FROM SolicitudBeca;
    ```

#### 7. **Instalación de PHP para Interfaz Gráfica para MySQL**

1. **Instala PHP y las extensiones necesarias**:
    ```bash
    sudo apt update
    sudo apt install php php-mbstring php-zip php-gd php-json php-curl
    ```

2. **Instala phpMyAdmin**:
    ```bash
    sudo apt install phpmyadmin
    ```

3. **Configura Apache2 para phpMyAdmin**:
    Si no tienes Apache2 instalado:
    ```bash
    sudo apt install apache2
    ```
    Crea un enlace simbólico para acceder a phpMyAdmin:
    ```bash
    sudo ln -s /usr/share/phpmyadmin /var/www/html/phpmyadmin
    ```

4. **Accede a phpMyAdmin** desde el navegador:
    ```
    http://localhost/phpmyadmin
    ```

#### 8. **Instalación de MySQL Connector/C++**

1. **Instala las dependencias de MySQL**:
    ```bash
    sudo apt update
    sudo apt install libmysqlclient-dev
    ```

2. **Instala MySQL Connector/C++**:
    ```bash
    sudo apt install libmysqlcppconn-dev
    ```

3. **Incluye la librería en tu proyecto C++**:
    ```cpp
    #include <mysql_driver.h>
    #include <mysql_connection.h>
    #include <cppconn/prepared_statement.h>
    ```
    
4. **Asegurate que esté correctamente configurado el CMakeList de System para el correcto uso**