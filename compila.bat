@chcp 1252 >nul

@if not exist game.c @echo:Deve haver um arquivo chamado game.c
@if not exist game.c pause
@if not exist game.c goto :eof

@if exist executavel.exe del executavel.exe
@poly\tinyCC\tcc.exe game.c poly\poly.c -w -o executavel.exe

@if not exist executavel.exe @echo:
@if not exist executavel.exe @echo:O código fonte não está de acordo com as regras da linguagem C

@if exist executavel.exe @cls
@if exist executavel.exe @echo:O arquivo executavel está disponível caso você queira compartilhar o jogo !
@echo:

@pause
@if exist executavel.exe executavel.exe
