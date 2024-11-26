CREATE DATABASE Biblioteca;
USE Biblioteca;


CREATE TABLE Usuarios (
    id_usuario INT AUTO_INCREMENT PRIMARY KEY,
    nome_usuario VARCHAR(50) NOT NULL UNIQUE,
    email VARCHAR(100) NOT NULL UNIQUE,
    data_nascimento DATE NOT NULL,
    papel_na_biblioteca ENUM('Bibliotecário', 'Convidado') NOT NULL,
	tags ENUM('Fixer', 'Bibliotecario', 'Gerente', 'Cidadao') NOT NULL,
    data_cadastro DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE Conexoes (
    id_conexao INT AUTO_INCREMENT PRIMARY KEY,
    id_usuario1 INT NOT NULL,	
    id_usuario2 INT NOT NULL,
    status_conexao ENUM('Próximo', 'Conhecido', 'Desconhecido') NOT NULL,
    FOREIGN KEY (id_usuario1) REFERENCES Usuarios(id_usuario) ON DELETE CASCADE,
    FOREIGN KEY (id_usuario2) REFERENCES Usuarios(id_usuario) ON DELETE CASCADE
);

CREATE TABLE Livros (
    id_livro INT AUTO_INCREMENT PRIMARY KEY,
    nome_livro VARCHAR(255) NOT NULL,
    id_usuario INT NOT NULL,
    FOREIGN KEY (id_usuario) REFERENCES Usuarios(id_usuario) ON DELETE CASCADE
);


CREATE TABLE Postagens (
    id_postagem INT AUTO_INCREMENT PRIMARY KEY,
    id_usuario INT NOT NULL,
    tipo_postagem ENUM('Livro', 'Requisito', 'Página') NOT NULL,
    id_livro INT,
    conteudo TEXT NOT NULL,
    data_criacao DATETIME DEFAULT CURRENT_TIMESTAMP,
    avaliacao_positiva INT DEFAULT 0,
    avaliacao_negativa INT DEFAULT 0,
    FOREIGN KEY (id_usuario) REFERENCES Usuarios(id_usuario) ON DELETE CASCADE,
    FOREIGN KEY (id_livro) REFERENCES Livros(id_livro) ON DELETE SET NULL
);

CREATE TABLE Notificacoes (
    id_notificacao INT AUTO_INCREMENT PRIMARY KEY,
    id_usuario_destino INT NOT NULL,
    id_postagem INT NOT NULL,
    tipo_notificacao ENUM('Requisito', 'Interesse', 'Avaliação') NOT NULL,
    descricao TEXT NOT NULL,
    data_notificacao DATETIME DEFAULT CURRENT_TIMESTAMP,
    area_notificacao VARCHAR(255) NOT NULL,
    FOREIGN KEY (id_usuario_destino) REFERENCES Usuarios(id_usuario) ON DELETE CASCADE,
    FOREIGN KEY (id_postagem) REFERENCES Postagens(id_postagem) ON DELETE CASCADE
);


CREATE TABLE Grupos (
    id_grupo INT AUTO_INCREMENT PRIMARY KEY,
    nome_grupo VARCHAR(100) NOT NULL UNIQUE,
    descricao TEXT NOT NULL,
    id_lider INT NOT NULL,
    data_criacao DATETIME DEFAULT CURRENT_TIMESTAMP,
    limite_membros INT DEFAULT 10,
    FOREIGN KEY (id_lider) REFERENCES Usuarios(id_usuario) ON DELETE CASCADE
);
CREATE TABLE Mensagens (
    id_mensagem INT AUTO_INCREMENT PRIMARY KEY,
    id_usuario_origem INT NOT NULL,
    id_usuario_destino INT NOT NULL,
    conteudo TEXT NOT NULL,
    status ENUM('Enviada', 'Recebida', 'Lida') DEFAULT 'Enviada',
    data_envio DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (id_usuario_origem) REFERENCES Usuarios(id_usuario) ON DELETE CASCADE,
    FOREIGN KEY (id_usuario_destino) REFERENCES Usuarios(id_usuario) ON DELETE CASCADE
);


DELIMITER //

CREATE TRIGGER Notificar_Postagem_Após_Insert
AFTER INSERT ON Postagens
FOR EACH ROW
BEGIN
    -- Inserir uma notificação sobre a postagem
    INSERT INTO Notificacoes (id_usuario_destino, id_postagem, tipo_notificacao, descricao, area_notificacao)
    VALUES (NEW.id_usuario, NEW.id_postagem, 'Interesse', 
            CONCAT('O usuário ', NEW.id_usuario, ' fez uma postagem sobre ', NEW.tipo_postagem, ' - ', NEW.conteudo), 
            'Área de Postagens');
END//

DELIMITER ;

ALTER TABLE Usuarios MODIFY COLUMN tags VARCHAR(255);
