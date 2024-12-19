-- Eliminar manualmente si se ejecuta por segunda vez

-- Crear la tabla de 'Universidad' para almacenar las universidades
CREATE TABLE IF NOT EXISTS Universidad (
    universidad_id INT AUTO_INCREMENT PRIMARY KEY,  -- ID único para cada universidad
    nombre VARCHAR(255) NOT NULL,                    -- Nombre de la universidad
    direccion VARCHAR(255),                          -- Dirección de la universidad
    ciudad VARCHAR(255),                             -- Ciudad
    numero_maximo INT NOT NULL,                      -- Número máximo de plazas disponibles para SICUE
    numero_ocupadas INT DEFAULT 0,                   -- Número de plazas ocupadas (se actualiza con cada solicitud confirmada)
    maximo_alumnos INT NOT NULL,                     -- Número máximo de plazas para alumnos
    numero_ocupados_alumnos INT DEFAULT 0,           -- Número de plazas ocupadas por alumnos
    maximo_profesores INT NOT NULL,                  -- Número máximo de plazas para profesores
    numero_ocupados_profesores INT DEFAULT 0         -- Número de plazas ocupadas por profesores
);

-- Crear la tabla de 'Titulacion'
CREATE TABLE IF NOT EXISTS Titulacion (
    titulacion_id INT AUTO_INCREMENT PRIMARY KEY,    -- ID único para cada titulación
    universidad_id INT NOT NULL,                     -- ID de la universidad a la que pertenece la titulación
    departamento VARCHAR(255) NOT NULL,              -- Nombre del departamento
    FOREIGN KEY (universidad_id) REFERENCES Universidad(universidad_id) ON DELETE CASCADE
);

-- Crear la tabla de 'User' (usuarios, ya sean admin, alumnos o profesores)
CREATE TABLE IF NOT EXISTS User (
    user_id INT AUTO_INCREMENT PRIMARY KEY,           -- ID único para cada usuario
    nombre VARCHAR(255) NOT NULL,                     -- Nombre del usuario
    password VARCHAR(255) NOT NULL,                   -- Contraseña del usuario
    tipo_user ENUM('admin', 'alumno', 'profesor') NOT NULL,  -- Tipo de usuario
    curso_academico VARCHAR(255),                     -- Año o periodo académico
    fecha_creacion TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- Fecha de creación
    universidad_origen INT,                           -- ID de universidad origen
    universidad_destino INT,                          -- ID de universidad destino
    FOREIGN KEY (universidad_origen) REFERENCES Universidad(universidad_id),
    FOREIGN KEY (universidad_destino) REFERENCES Universidad(universidad_id)
);

-- Crear la tabla de 'Alumno' que hereda de 'User'
CREATE TABLE IF NOT EXISTS Alumno (
    alumno_id INT AUTO_INCREMENT PRIMARY KEY,        -- ID único para cada alumno
    user_id INT NOT NULL,                            -- Referencia al ID de 'User'
    carrera VARCHAR(255),                            -- Carrera del alumno
    FOREIGN KEY (user_id) REFERENCES User(user_id) ON DELETE CASCADE
);

-- Crear la tabla de 'Profesor' que hereda de 'User'
CREATE TABLE IF NOT EXISTS Profesor (
    profesor_id INT AUTO_INCREMENT PRIMARY KEY,      -- ID único para cada profesor
    user_id INT NOT NULL,                            -- Referencia al ID de 'User'
    departamento VARCHAR(255) NOT NULL,               -- Departamento al que pertenece el profesor
    FOREIGN KEY (user_id) REFERENCES User(user_id) ON DELETE CASCADE
);

-- Crear la tabla de 'Solicitud' base
CREATE TABLE IF NOT EXISTS Solicitud (
    solicitud_id INT AUTO_INCREMENT PRIMARY KEY,     -- ID único para cada solicitud
    solicitante_id INT NOT NULL,                     -- Referencia al ID del solicitante (Alumno o Profesor)
    tipo_solicitante ENUM('Estudiante', 'Profesor') NOT NULL,  -- Tipo de solicitante
    universidad_origen VARCHAR(255),                 -- Universidad de origen
    universidad_destino VARCHAR(255),                -- Universidad de destino
    departamento VARCHAR(255),                       -- Departamento o carrera
    curso VARCHAR(255),                              -- Curso académico al que aplica
    estado_solicitud ENUM('Pendiente', 'Aceptada', 'Rechazada', 'Inactiva') DEFAULT 'Pendiente', -- Estado de la solicitud
    fecha_solicitud TIMESTAMP DEFAULT CURRENT_TIMESTAMP,  -- Fecha en la que se realiza la solicitud
    FOREIGN KEY (solicitante_id) REFERENCES User(user_id) ON DELETE CASCADE
);

-- Crear la tabla de 'SolicitudSicue' que hereda de 'Solicitud'
CREATE TABLE IF NOT EXISTS SolicitudSicue (
    solicitud_id INT PRIMARY KEY,                    -- ID de solicitud (referencia de 'Solicitud')
    universidad_origen VARCHAR(255),                  -- Universidad de origen
    universidad_destino VARCHAR(255),                 -- Universidad de destino
    FOREIGN KEY (solicitud_id) REFERENCES Solicitud(solicitud_id) ON DELETE CASCADE
);

-- Crear la tabla de 'SolicitudBeca' que hereda de 'Solicitud' (solo para alumnos)
CREATE TABLE IF NOT EXISTS SolicitudBeca (
    solicitud_id INT PRIMARY KEY,                    -- ID de solicitud (referencia de 'Solicitud')
    monto_beca DECIMAL(10, 2),                        -- Monto de la beca solicitada
    tipo_beca VARCHAR(255),                           -- Tipo de beca solicitada
    FOREIGN KEY (solicitud_id) REFERENCES Solicitud(solicitud_id) ON DELETE CASCADE
);
-- Insertar datos de ejemplo en la tabla 'Universidad'
INSERT INTO Universidad (nombre, direccion, ciudad, numero_maximo, maximo_alumnos, maximo_profesores) VALUES
('Universidad A', 'Calle 123, Ciudad A', 'Ciudad A', 100, 50, 10),
('Universidad B', 'Calle 456, Ciudad B', 'Ciudad B', 150, 70, 20);

-- Insertar datos en la tabla 'User'
INSERT INTO User (nombre, password, tipo_user, universidad_origen, universidad_destino, curso_academico) VALUES
('Juan Pérez', 'password123', 'alumno', 1, 2, '2024-2025'),
('María Gómez', 'password456', 'alumno', 2, 1, '2024-2025'),
('Carlos Díaz', 'password789', 'profesor', 1, 2, '2024-2025');

-- Insertar datos en la tabla 'Alumno'
INSERT INTO Alumno (user_id, carrera) VALUES
(1, 'Ingeniería Informática'),
(2, 'Matemáticas');

-- Insertar datos en la tabla 'Profesor'
INSERT INTO Profesor (user_id, departamento) VALUES
(3, 'Informática');

-- Insertar datos en la tabla 'Solicitud'
INSERT INTO Solicitud (solicitante_id, tipo_solicitante, universidad_origen, universidad_destino, departamento, curso, estado_solicitud) VALUES
(1, 'Estudiante', 'Universidad A', 'Universidad B', 'Informática', '2024-2025', 'Pendiente'),
(2, 'Estudiante', 'Universidad B', 'Universidad A', 'Matemáticas', '2024-2025', 'Pendiente'),
(3, 'Profesor', 'Universidad A', 'Universidad B', 'Informática', '2024-2025', 'Pendiente');

-- Insertar datos en la tabla 'SolicitudSicue'
INSERT INTO SolicitudSicue (solicitud_id, universidad_origen, universidad_destino) VALUES
(1, 'Universidad A', 'Universidad B'),
(2, 'Universidad B', 'Universidad A');

-- Insertar datos en la tabla 'SolicitudBeca'
INSERT INTO SolicitudBeca (solicitud_id, monto_beca, tipo_beca) VALUES
(1, 1500.00, 'Beca Completa'),
(2, 1000.00, 'Beca Parcial');

-- Procedimiento para actualizar el número de plazas ocupadas al confirmar una solicitud SICUE
DELIMITER $$

CREATE PROCEDURE ActualizarPlazasOcupadas(IN solicitud_id INT, IN tipo_solicitante ENUM('Estudiante', 'Profesor'))
BEGIN
    DECLARE universidad_destino_id INT;
    
    -- Obtener el ID de la universidad de destino de la solicitud
    SELECT universidad_destino INTO universidad_destino_id FROM Solicitud WHERE solicitud_id = solicitud_id;
    
    -- Actualizar el número de plazas ocupadas según el tipo de solicitante
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

-- Consultar todas las solicitudes
SELECT * FROM Solicitud;

-- Consultar todas las solicitudes SICUE
SELECT * FROM SolicitudSicue;

-- Consultar todas las solicitudes de beca
SELECT * FROM SolicitudBeca;