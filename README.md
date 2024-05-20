# Sobre o Projeto
- Esse projeto teve o objetivo de programar um Shell (interpretador de comandos) na linguagem C e aprimorar conceitos aprendidos em Sistemas Operacionais.


# Tecnologias Utilizadas
- Visual Studio Code: plataforma escolhida para desenvolver o código.
- Oracle VM VirtualBox: utilizamos uma VM Linux para conseguir compilar nosso código.


# Como usar o Shell
Após abrir e configurar sua VM Linux, abra um terminal e utilize esses comandos:

 sudo mount -t vboxsf labs labs
  - O primeiro “labs” é o nome do compartilhamento configurado no seu sistema e o segundo “labs” é o caminho
para a pasta recém criada.

 sudo apt install build-essential
  - Nesse comando, apt é um gerenciador de pacotes popular em distribuições Linux baseadas em Debian, como é o
caso do Ubuntu. Com apt install, estamos instruindo a instalação de um pacote, no caso, build-essential,
que contém ferramentas de compilação e bibliotecas essenciais para trabalhar com linguagem C. A instalação de
pacotes é uma tarefa que requer privilégios administrativos, por isso a necessidade de sudo.

 pwd
 ls
 cd labs
 ls -lha
 cat projeto.c
  - O comando pwd mostra o caminho para o diretório atual, ls lista o diretório. O modificador -lha no ls instrui para
que a listagem seja feita em forma de lista (-l), utilizando tamanhos de diretórios legíveis para humanos (-h) e
incluindo todo o que há no diretório (-a). Com cd, navegamos para outro diretório (utilize cd .. para voltar ao
diretório anterior) e cat lê e direciona para a saída o conteúdo de um arquivo.


# E para compilar o programa:
- gcc -c projeto.c 
- gcc -o projeto projeto.o
- ./projeto


# Comandos internos:
- cd
- path
- exit 

# para utilizar os comandos externos (cat, ls, etc...):
path /bin /usr/bin


Obs: qualquer dúvida, utilizar o pdf Lab0 do professor Isaías.
