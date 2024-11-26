USE biblioteca;
SELECT u1.nome_usuario AS `Usuario principal` , u2.nome_usuario AS `Com quem conecta`, c.status_conexao as  `status` FROM Conexoes c 
JOIN Usuarios u1 ON c.id_usuario1 = u1.id_usuario
JOIN Usuarios u2 ON c.id_usuario2 = u2.id_usuario
WHERE u1.nome_usuario = 'Roland'
ORDER BY CASE WHEN c.status_conexao = 'Próximo' THEN 1 WHEN c.status_conexao = 'Conhecido' THEN 2 ELSE 3 END, u2.nome_usuario
LIMIT 0, 1000;
# ------------------------------- 
USE biblioteca;
SELECT u.nome_usuario AS Usuario, c.status_conexao AS `status`, COUNT(*) AS `Total de conexoes` 
FROM Conexoes c
JOIN Usuarios u ON c.id_usuario1 = u.id_usuario
GROUP BY u.nome_usuario, c.status_conexao
ORDER BY 3 DESC; 
# ------------------------------- 
USE biblioteca;
SELECT u.nome_usuario AS Usuario, p.tipo_postagem as Tipo , p.conteudo, p.data_criacao  as data
FROM Postagens p 
JOIN Usuarios u ON p.id_usuario = u.id_usuario 
WHERE u.nome_usuario = 'Roland' 
ORDER BY p.data_criacao DESC;
#-------------------
USE biblioteca; 
SELECT g.nome_grupo AS Grupo, g.descricao AS Descricao, u.nome_usuario AS Lider, g.data_criacao AS Data_Criacao 
FROM Grupos g 
JOIN Usuarios u ON g.id_lider = u.id_usuario 
ORDER BY g.id_lider ASC;
#-------------------
USE biblioteca;
SELECT u.nome_usuario AS Usuario, l.nome_livro AS Livro 
FROM Livros l
JOIN Usuarios u ON l.id_usuario = u.id_usuario
ORDER BY u.id_usuario ASC;
