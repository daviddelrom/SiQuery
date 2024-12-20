-- Eliminar las tablas si existen
DROP TABLE IF EXISTS SolicitudSicueALUMNO;
DROP TABLE IF EXISTS SolicitudSicueProfesor;
DROP TABLE IF EXISTS SolicitudBeca;
DROP TABLE IF EXISTS Profesor;
DROP TABLE IF EXISTS Alumno;
DROP TABLE IF EXISTS User;
DROP TABLE IF EXISTS Titulacion;
DROP TABLE IF EXISTS Universidad;
DROP TABLE IF EXISTS Carrera;

CREATE TABLE IF NOT EXISTS Carrera(
    titulacion_id INT AUTO_INCREMENT PRIMARY KEY,    -- ID único para cada titulación
    universidad_id INT,                              -- ID de la universidad (no relacional)
    carrera VARCHAR(255) NOT NULL,
    anioCurso INT NOT NULL,
    campo VARCHAR(255) NOT NULL
);
-- Crear la tabla de 'Universidad'
CREATE TABLE IF NOT EXISTS Universidad (
    universidad_id INT AUTO_INCREMENT PRIMARY KEY,  -- ID único para cada universidad
    nombre VARCHAR(255) NOT NULL,                    -- Nombre de la universidad
    direccion VARCHAR(255),                          -- Dirección de la universidad
    ciudad VARCHAR(255),                             -- Ciudad
    maximo_alumnos INT NOT NULL,                     -- Número máximo de plazas para alumnos
    numero_ocupados_alumnos INT DEFAULT 0,           -- Número de plazas ocupadas por alumnos
    maximo_profesores INT NOT NULL,                  -- Número máximo de plazas para profesores
    numero_ocupados_profesores INT DEFAULT 0         -- Número de plazas ocupadas por profesores
);

-- Crear la tabla de 'Titulacion' sin claves foráneas
CREATE TABLE IF NOT EXISTS Titulacion (
    titulacion_id INT AUTO_INCREMENT PRIMARY KEY,    -- ID único para cada titulación
    universidad_id INT,                              -- ID de la universidad (no relacional)
    departamento VARCHAR(255) NOT NULL              -- Nombre del departamento
);

-- Crear la tabla de 'User'
CREATE TABLE IF NOT EXISTS User (
    user_id INT AUTO_INCREMENT PRIMARY KEY,          -- ID único para cada usuario
    nombre VARCHAR(255) NOT NULL,                    -- Nombre del usuario
    password VARCHAR(255) NOT NULL,                  -- Contraseña del usuario
    tipo_user ENUM('admin', 'alumno', 'profesor') NOT NULL,  -- Tipo de usuario
    curso_academico VARCHAR(255),                    -- Año o periodo académico
    universidad_origen INT,                          -- ID de universidad origen (no relacional)
    universidad_destino INT                          -- ID de universidad destino (no relacional)
);

CREATE TABLE IF NOT EXISTS Alumno (
    alumno_id INT AUTO_INCREMENT PRIMARY KEY,        -- ID único para cada alumno
    user_id INT,                                     -- Referencia manual al ID de 'User'
    carrera VARCHAR(255),                             -- Carrera del alumno
    año_academico INT                       
);


-- Crear la tabla de 'Profesor'
CREATE TABLE IF NOT EXISTS Profesor (
    profesor_id INT AUTO_INCREMENT PRIMARY KEY,      -- ID único para cada profesor
    user_id INT,                                     -- Referencia manual al ID de 'User'
    departamento VARCHAR(255) NOT NULL              -- Departamento al que pertenece el profesor
);

CREATE TABLE IF NOT EXISTS SolicitudSicueProfesor (
    solicitud_id INT AUTO_INCREMENT PRIMARY KEY,
    nombre_solicitante VARCHAR(255) NOT NULL,
    solicitante_id INT,
    universidad_origen VARCHAR(255),  
    universidad_destino VARCHAR(255),
    departamento VARCHAR(255), 
    curso VARCHAR(255),
    estado_solicitud ENUM('Pendiente', 'Aceptada', 'Rechazada', 'Inactiva') DEFAULT 'Pendiente',
    fecha_solicitud TIMESTAMP DEFAULT CURRENT_TIMESTAMP 
);

CREATE TABLE IF NOT EXISTS SolicitudSicueALUMNO (
    solicitud_id INT AUTO_INCREMENT PRIMARY KEY,
    nombre_solicitante VARCHAR(255) NOT NULL,
    solicitante_id INT,
    universidad_origen VARCHAR(255),  
    universidad_destino VARCHAR(255),
    carrera VARCHAR(255), 
    curso VARCHAR(255),
    estado_solicitud ENUM('Pendiente', 'Aceptada', 'Rechazada', 'Inactiva') DEFAULT 'Pendiente',
    fecha_solicitud TIMESTAMP DEFAULT CURRENT_TIMESTAMP 
);

CREATE TABLE IF NOT EXISTS SolicitudBeca(
    solicitud_id INT AUTO_INCREMENT PRIMARY KEY,
    nombre_solicitante VARCHAR(255) NOT NULL,
    solicitante_id INT,
    carrera VARCHAR(255), 
    curso VARCHAR(255), 
    estado_solicitud ENUM('Pendiente', 'Aceptada', 'Rechazada', 'Inactiva') DEFAULT 'Pendiente',
    fecha_solicitud TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Inserciones para la tabla Carrera con la asociación correcta de universidad_id y campo
INSERT INTO Carrera (universidad_id, carrera, anioCurso, campo) VALUES
(1, 'Ingeniería Informática', 1, 'Ingeniería'),
(1, 'Ingeniería Informática', 2, 'Ingeniería'),
(1, 'Ingeniería Informática', 3, 'Ingeniería'),
(1, 'Ingeniería Informática', 4, 'Ingeniería'),

(2, 'Biología', 1, 'Ciencias de la Salud'),
(2, 'Biología', 2, 'Ciencias de la Salud'),
(2, 'Biología', 3, 'Ciencias de la Salud'),
(2, 'Biología', 4, 'Ciencias de la Salud'),

(3, 'Medicina', 1, 'Ciencias de la Salud'),
(3, 'Medicina', 2, 'Ciencias de la Salud'),
(3, 'Medicina', 3, 'Ciencias de la Salud'),
(3, 'Medicina', 4, 'Ciencias de la Salud'),

(4, 'Derecho', 1, 'Derecho y Ciencias Políticas'),
(4, 'Derecho', 2, 'Derecho y Ciencias Políticas'),
(4, 'Derecho', 3, 'Derecho y Ciencias Políticas'),
(4, 'Derecho', 4, 'Derecho y Ciencias Políticas'),

(5, 'Filosofía', 1, 'Ciencias Sociales'),
(5, 'Filosofía', 2, 'Ciencias Sociales'),
(5, 'Filosofía', 3, 'Ciencias Sociales'),
(5, 'Filosofía', 4, 'Ciencias Sociales'),

(6, 'Psicología', 1, 'Ciencias Sociales'),
(6, 'Psicología', 2, 'Ciencias Sociales'),
(6, 'Psicología', 3, 'Ciencias Sociales'),
(6, 'Psicología', 4, 'Ciencias Sociales'),

(7, 'Administración de Empresas', 1, 'Negocios y Economía'),
(7, 'Administración de Empresas', 2, 'Negocios y Economía'),
(7, 'Administración de Empresas', 3, 'Negocios y Economía'),
(7, 'Administración de Empresas', 4, 'Negocios y Economía'),

(8, 'Ingeniería Civil', 1, 'Ingeniería'),
(8, 'Ingeniería Civil', 2, 'Ingeniería'),
(8, 'Ingeniería Civil', 3, 'Ingeniería'),
(8, 'Ingeniería Civil', 4, 'Ingeniería'),

(9, 'Matemáticas', 1, 'Ciencias Exactas'),
(9, 'Matemáticas', 2, 'Ciencias Exactas'),
(9, 'Matemáticas', 3, 'Ciencias Exactas'),
(9, 'Matemáticas', 4, 'Ciencias Exactas'),

(10, 'Ingeniería Industrial', 1, 'Ingeniería');

-- Inserciones para la tabla Titulacion con la universidad_id correspondiente
INSERT INTO Titulacion (universidad_id, departamento) VALUES
(1, 'Computación'),
(2, 'Salud'),
(3, 'Diseño y Urbanismo'),
(3, 'Computación'),
(4, 'Derecho y Ciencias Políticas'),
(5, 'Ciencias Naturales'),
(6, 'Ciencias Sociales'),
(7, 'Negocios y Economía'),
(8, 'Ingeniería y Tecnología'),
(9, 'Humanidades'),
(10, 'Ciencias Exactas'),
(1, 'Ingeniería Electrónica'),
(7, 'Economía'),
(6, 'Psicología Clínica');

-- Inserciones para la tabla User con las universidades y tipos de usuario correctamente asignados
INSERT INTO User (nombre, password, tipo_user, curso_academico, universidad_origen, universidad_destino) VALUES 
('Juan Pérez', 'password123', 'alumno', '1', 1, 2), 
('Laura Sánchez', 'password321', 'alumno', '2', 3, 4), 
('Carlos García', 'password456', 'profesor', '3', 5, 6), 
('Ana López', 'password654', 'alumno', '4', 7, 8), 
('Pedro Martínez', 'password789', 'profesor', '2', 9, 10), 
('María Rodríguez', 'password987', 'alumno', '1', 2, 1), 
('Luis Fernández', 'password654', 'profesor', '4', 6, 7), 
('Pedro López', 'password321', 'alumno', '3', 8, 9), 
('Isabel García', 'password123', 'alumno', '2', 10, 5), 
('Raúl Pérez', 'password000', 'profesor', '4', 4, 6);

-- Inserciones para la tabla Alumno relacionando el `user_id` y las carreras con el año académico
INSERT INTO Alumno (user_id, carrera, año_academico) VALUES 
(1, 'Ingeniería Informática', 1), 
(2, 'Medicina', 2), 
(3, 'Arquitectura', 3), 
(4, 'Derecho', 4), 
(5, 'Biología', 1), 
(6, 'Psicología', 2), 
(7, 'Administración de Empresas', 3), 
(8, 'Ingeniería Civil', 4), 
(9, 'Filosofía', 1), 
(10, 'Matemáticas', 2);

-- Inserciones para la tabla Profesor
INSERT INTO Profesor (user_id, departamento) VALUES 
(1, 'Computación'),
(2, 'Salud'),
(3, 'Diseño y Urbanismo'),
(4, 'Derecho y Ciencias Políticas'),
(5, 'Ciencias Naturales'),
(6, 'Ciencias Sociales'),
(7, 'Negocios y Economía'),
(8, 'Ingeniería y Tecnología'),
(9, 'Humanidades'),
(10, 'Ciencias Exactas'),
(1, 'Ingeniería Electrónica'),
(7, 'Economía');
