// Enumeração com todos os códigos de teclas suportadas (Windows virtual key codes)
enum keycode_t{
	KEY_INVALID,KEY_LBUTTON,KEY_RBUTTON,KEY_CANCEL,KEY_MBUTTON,KEY_XBUTTON1,KEY_XBUTTON2,KEY_BACK,KEY_TAB,
	KEY_CLEAR,KEY_RETURN,KEY_SHIFT,KEY_CONTROL,KEY_MENU,KEY_PAUSE,KEY_CAPITAL,KEY_KANA,KEY_HANGUL=KEY_KANA,
	KEY_JUNJA,KEY_FINAL,KEY_HANJA,KEY_KANJI=KEY_HANJA,KEY_ESCAPE,KEY_CONVERT,KEY_NONCONVERT,KEY_ACCEPT,
	KEY_MODECHANGE,KEY_SPACE,KEY_PRIOR,KEY_NEXT,KEY_END,KEY_HOME,KEY_LEFT,KEY_UP,KEY_RIGHT,KEY_DOWN,
	KEY_SELECT,KEY_PRINT,KEY_EXEC,KEY_SNAPSHOT,KEY_INSERT,KEY_DELETE,KEY_HELP,KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,
	KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_A,KEY_B,KEY_C,KEY_D,KEY_E,KEY_F,KEY_G,KEY_H,KEY_I,KEY_J,KEY_K,
	KEY_L,KEY_M,KEY_N,KEY_O,KEY_P,KEY_Q,KEY_R,KEY_S,KEY_T,KEY_U,KEY_V,KEY_W,KEY_X,KEY_Y,KEY_Z,KEY_LWIN,
	KEY_RWIN,KEY_APPS,KEY_SLEEP,KEY_NUMPAD0,KEY_NUMPAD1,KEY_NUMPAD2,KEY_NUMPAD3,KEY_NUMPAD4,KEY_NUMPAD5,
	KEY_NUMPAD6,KEY_NUMPAD7,KEY_NUMPAD8,KEY_NUMPAD9,KEY_MULTIPLY,KEY_ADD,KEY_SEPARATOR,KEY_SUBTRACT,KEY_DECIMAL,
	KEY_DIVIDE,KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11,KEY_F12,
	KEY_F13,KEY_F14,KEY_F15,KEY_F16,KEY_F17,KEY_F18,KEY_F19,KEY_F20,KEY_F21,KEY_F22,KEY_F23,KEY_F24,
	KEY_NUMLOCK,KEY_SCROLL,KEY_LSHIFT,KEY_RSHIFT,KEY_LCONTROL,KEY_RCONTROL,KEY_LMENU,KEY_RMENU,KEY_BROWSER_BACK,
	KEY_BROWSER_FORWARD,KEY_BROWSER_REFRESH,KEY_BROWSER_STOP,KEY_BROWSER_SEARCH,KEY_BROWSER_FAVORITES,
	KEY_BROWSER_HOME,KEY_VOLUME_MUTE,KEY_VOLUME_DOWN,KEY_VOLUME_UP,KEY_MEDIA_NEXT_TRACK,KEY_MEDIA_PREV_TRACK,
	KEY_MEDIA_STOP,KEY_MEDIA_PLAY_PAUSE,KEY_LAUNCH_MAIL,KEY_LAUNCH_MEDIA_SELECT,KEY_LAUNCH_APP1,KEY_LAUNCH_APP2,
	KEY_OEM_1,KEY_OEM_PLUS,KEY_OEM_COMMA,KEY_OEM_MINUS,KEY_OEM_PERIOD,KEY_OEM_2,KEY_OEM_3,KEY_OEM_4,KEY_OEM_5,
	KEY_OEM_6,KEY_OEM_7,KEY_OEM_8,KEY_OEM_102,KEY_PROCESSKEY,KEY_ATTN,KEY_CRSEL,KEY_EXSEL,KEY_EREOF,KEY_PLAY,
	KEY_ZOOM,KEY_NONAME,KEY_PA1,KEY_OEM_CLEAR,
	KEYCOUNT,KEYPADDING=0xFFFFFFFF
};

#ifdef POLY /* POLY */
	#ifndef NO_MAIN_DEF
		#ifdef main
			#undef main
		#endif
	#endif

	#define _CRT_NONSTDC_NO_DEPRECATE
	#define _CRT_SECURE_NO_WARNINGS
	#include <stdbool.h>
	#include <stdint.h>
	#include <ctype.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include "app.h"
	#include "crtemu_pc.h"
	#include "frametimer.h"
	#include "thread.h"

// vertical blanking interval - sincroniza a execução da thread do usuário com a taxa de atualização do monitor (60Hz)
struct{
	thread_signal_t signal; // Sinal que acorda threads esperando pelo VBL
	thread_atomic_int_t count; // Contador que incrementa a cada sincronização vertical
}vbl;

// Gerencia o buffer de vídeo e a paleta de cores
struct{
	int width;
	int height;
	int color;
	uint8_t buffer[1024*1024];
	uint32_t palette[256];
}screen;

// aponta para a área de desenho - visão simplificada do screen que facilita operações de desenho
struct{
	uint8_t* buffer;
	int width;
	int height;
}draw;

struct{
	bool keystate[KEYCOUNT]; // Array persistente com estado de cada tecla (true=pressionada)
	enum keycode_t* keybuffer; // Ponteiro para buffer ativo de teclas pressionadas/liberadas
	enum keycode_t keybuffer0[256]; // Buffer primário de eventos de teclado
	enum keycode_t keybuffer1[256]; // Buffer secundário (alternado para evitar race conditions)
	char* charbuffer; // Ponteiro para buffer ativo de caracteres digitados
	char charbuffer0[256]; // Buffer primário de caracteres ASCII
	char charbuffer1[256]; // Buffer secundário de caracteres ASCII
}input;

// passado para a thread do usuário (polymain) mantendo argc/argv
struct app_context_t{
	int argc;
	char** argv;
};

// Contexto da thread do usuário que gerencia sincronização entre a thread principal e a thread de usuário
// Controla inicialização, execução e término da função polymain()
struct user_thread_context_t{
	struct app_context_t* app_context; // Referência ao contexto da aplicação (argc/argv)
	thread_signal_t user_thread_initialized; // Sinal que indica quando a thread do usuário foi inicializada
	thread_atomic_int_t user_thread_finished; // Flag atômica: 1 quando polymain() termina
	thread_signal_t app_loop_finished; // Sinal que indica quando o loop principal terminou
	thread_signal_t user_thread_terminated; // Sinal que indica quando a thread do usuário completamente terminou
};

// Paleta de cores padrão com 256 cores em formato XBGR (24 bits)
static uint32_t default_palette[256]={
	0x000000,0xaa0000,0x00aa00,0xaaaa00,0x0000aa,0xaa00aa,0x0055aa,0xaaaaaa,0x555555,0xff5555,0x55ff55,0xffff55,0x5555ff,0xff55ff,0x55ffff,0xffffff,0x000000,0x141414,0x202020,0x2c2c2c,0x383838,0x454545,0x515151,0x616161,0x717171,0x828282,0x929292,0xa2a2a2,0xb6b6b6,0xcbcbcb,0xe3e3e3,0xffffff,0xff0000,0xff0041,0xff007d,0xff00be,0xff00ff,0xbe00ff,0x7d00ff,0x4100ff,0x0000ff,0x0041ff,0x007dff,0x00beff,0x00ffff,0x00ffbe,0x00ff7d,0x00ff41,0x00ff00,0x41ff00,0x7dff00,0xbeff00,0xffff00,0xffbe00,0xff7d00,0xff4100,0xff7d7d,0xff7d9e,0xff7dbe,0xff7ddf,0xff7dff,0xdf7dff,0xbe7dff,0x9e7dff,
	0x7f7dff,0x7d9eff,0x7dbeff,0x7ddfff,0x7dffff,0x7dffdf,0x7dffbe,0x7dff9e,0x7dff7d,0x9eff7d,0xbeff7d,0xdfff7d,0xffff7d,0xffdf7d,0xffbe7d,0xff9e7d,0xffb6b6,0xffb6c7,0xffb6db,0xffb6eb,0xffb6ff,0xebb6ff,0xdbb6ff,0xc7b6ff,0xb6b6ff,0xb6c7ff,0xb6dbff,0xb6ebff,0xb6ffff,0xb6ffeb,0xb6ffdb,0xb6ffc7,0xb6ffb6,0xc7ffb6,0xdbffb6,0xebffb6,0xffffb6,0xffebb6,0xffdbb6,0xffc7b6,0x710000,0x71001c,0x710038,0x710055,0x710071,0x550071,0x380071,0x1c0071,0x000071,0x001c71,0x003871,0x005571,0x007171,0x007155,0x007138,0x00711c,0x007100,0x1c7100,0x387100,0x557100,0x717100,0x715500,0x713800,0x711c00,
	0x713838,0x713845,0x713855,0x713861,0x713871,0x613871,0x553871,0x453871,0x383871,0x384571,0x385571,0x386171,0x387171,0x387161,0x387155,0x387145,0x387138,0x457138,0x557138,0x617138,0x717138,0x716138,0x715538,0x714538,0x715151,0x715159,0x715161,0x715169,0x715171,0x695171,0x615171,0x595171,0x515171,0x515971,0x516171,0x516971,0x517171,0x517169,0x517161,0x517159,0x517151,0x597151,0x617151,0x697151,0x717151,0x716951,0x716151,0x715951,0x410000,0x410010,0x410020,0x410030,0x410041,0x300041,0x200041,0x100041,0x000041,0x001041,0x002041,0x003041,0x004141,0x004130,0x004120,0x004110,
	0x004100,0x104100,0x204100,0x304100,0x414100,0x413000,0x412000,0x411000,0x412020,0x412028,0x412030,0x412038,0x412041,0x382041,0x302041,0x282041,0x202041,0x202841,0x203041,0x203841,0x204141,0x204138,0x204130,0x204128,0x204120,0x284120,0x304120,0x384120,0x414120,0x413820,0x413020,0x412820,0x412c2c,0x412c30,0x412c34,0x412c3c,0x412c41,0x3c2c41,0x342c41,0x302c41,0x2c2c41,0x2c3041,0x2c3441,0x2c3c41,0x2c4141,0x2c413c,0x2c4134,0x2c4130,0x2c412c,0x30412c,0x34412c,0x3c412c,0x41412c,0x413c2c,0x41342c,0x41302c,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
};

thread_atomic_int_t exit_flag;

// Altera o modo de vídeo para 320x200 pixels
void setvideomode(){
	memcpy(screen.palette,default_palette,1024);
	screen.width=320;
	screen.height=200;
	draw.buffer=screen.buffer;
	draw.width=screen.width;
	draw.height=screen.height;
};

// Aguarda o sinal de sincronização vertical (VBL) para sincronizar com o refresh da tela
void waitvbl(){
	if(thread_atomic_int_load(&exit_flag)==0){
		int current_vbl_count=thread_atomic_int_load(&vbl.count);
		while(current_vbl_count==thread_atomic_int_load(&vbl.count))thread_signal_wait(&vbl.signal,1000);
	}
}

// Incrementa o contador de VBL e sinaliza threads esperando pela sincronização
static void signalvbl(){
	thread_atomic_int_inc(&vbl.count);
	thread_signal_raise(&vbl.signal);
}

// Retorna o estado de uma tecla específica (true se pressionada, false caso contrário)
int keystate(enum keycode_t key){
	int index=(int)key;
	if(index>=0&&index<KEYCOUNT)return input.keystate[index];
	return false;
}

// Alterna entre dois buffers de entrada de teclado para evitar condições de corrida
enum keycode_t* readkeys(){
	memset(input.keybuffer,0,sizeof(input.keybuffer0));
	if(input.keybuffer==input.keybuffer0)input.keybuffer=input.keybuffer1;
	else input.keybuffer=input.keybuffer0;
	return input.keybuffer;
};

int polymain(int argc,char* argv[]); //assinatura da função placeholder da main do game.c

// Esta função é executada em uma thread separada (não na thread principal)
// Responsável por inicializar o estado global, executar polymain(), e sincronizar com a thread principal
static int user_thread_proc(void* user_data){
	struct user_thread_context_t* context=(struct user_thread_context_t*)user_data; // contexto da aplicação passado como argumento
	// Inicializa sistema de sincronização VBL
	thread_signal_init(&vbl.signal);
	thread_atomic_int_store(&vbl.count,0);
	// Inicializa estado padrão de vídeo (80x25 texto compatível com DOS)
	screen.width=80;
	screen.height=25;
	memcpy(screen.palette,default_palette,1024); // Copia paleta padrão de 256 cores
	draw.buffer=screen.buffer; // Aponta viewport para buffer ativo
	draw.width=screen.width;
	draw.height=screen.height;
	screen.color=15; // Define cor de desenho padrão (branco)
	// Inicializa buffers de entrada de teclado e caracteres
	input.keybuffer=input.keybuffer0;
	input.charbuffer=input.charbuffer0;
	// Sinaliza a thread principal que esta thread foi inicializada com sucesso
	thread_signal_raise(&context->user_thread_initialized);
	// Aguarda primeiro sinal de sincronização vertical antes de começar polymain
	waitvbl();
	// Executa a função principal do usuário com argumentos de linha de comando
	int result=polymain(context->app_context->argc,context->app_context->argv);
	// Marca que a thread do usuário terminou sua execução
	thread_atomic_int_store(&context->user_thread_finished,1); // a thread principal verifica este flag no loop principal
	// Aguarda sinal da thread principal (timeout 5000ms) indicando que o loop principal terminou
	thread_signal_wait(&context->app_loop_finished,5000); // isso garante que a thread principal processou o último frame e limpou buffers
	thread_signal_term(&vbl.signal); // libera recurso de sincronização VBL
	thread_signal_raise(&context->user_thread_terminated); // Sinaliza a thread principal que esta thread terminou completamente (permite que a thread principal continue com finalizações)
	return result;
}

//	Função principal da aplicação que gerencia o loop de eventos, entrada, renderização e sincronização
//		- app: Ponteiro para a estrutura da aplicação
//		- user_data: Dados do usuário contendo contexto da aplicação (argc/argv)
static int app_proc(app_t* app,void* user_data){
	struct app_context_t* app_context=(struct app_context_t*)user_data;

	// Define o título da janela com o nome do arquivo executável
	app_title(app,app_filename(app));
	bool fullscreen=true;

	// Processa argumentos de linha de comando
	// Remove flags de modo de tela (--window, -w) e mantém outros argumentos para polymain()
	int modargc=0;
	char* modargv[256];
	for(int i=0;i<app_context->argc;++i){
		if(strcmp(app_context->argv[i],"--window")==0)fullscreen=false; // Força modo janela
		else if(strcmp(app_context->argv[i],"-w")==0)fullscreen=false; // Versão curta de --window
		else{
			// Copia argumentos que não são flags de modo de tela
			if(modargc>=sizeof(modargv)/sizeof(*modargv))break;
			modargv[modargc++]=app_context->argv[i];
		}
	}
	// Atualiza contexto com argumentos filtrados
	app_context->argc=modargc;
	app_context->argv=modargv;

	// Configura modo de vídeo (fullscreen ou janela)
	app_screenmode(app,fullscreen?APP_SCREENMODE_FULLSCREEN:APP_SCREENMODE_WINDOW);
	// Esconde o cursor do mouse (usando buffer em branco)
	APP_U32 blank=0;
	app_pointer(app,1,1,&blank,0,0);

	// Obtém informações dos monitores disponíveis
	app_displays_t displays=app_displays(app);
	if(displays.count>0){
		// Encontra o monitor principal (coordenadas 0,0)
		int disp=0;
		for(int i=0;i<displays.count;++i)if(displays.displays[i].x==0&&displays.displays[i].y==0){
			disp=i;
			break;
		}

		// Calcula dimensões da janela com espaço de margem (80 pixels)
		int scrwidth=displays.displays[disp].width-80;
		int scrheight=displays.displays[disp].height-80;

		// Mantém proporção de aspecto 4:3 para 320x200 (4.25:3)
		int aspect_width=(int)((scrheight*4.25f)/3);
		int aspect_height=(int)((scrwidth*3)/4.25f);
		int target_width,target_height;

		// Escolhe dimensão que melhor se encaixa na tela
		if(aspect_height<=scrheight){
			target_width=scrwidth;
			target_height=aspect_height;
		}else{
			target_width=aspect_width;
			target_height=scrheight;
		}

		// Centraliza a janela no monitor
		int x=displays.displays[disp].x+(displays.displays[disp].width-target_width)/2;
		int y=displays.displays[disp].y+(displays.displays[disp].height-target_height)/2;
		int w=target_width;
		int h=target_height;
		app_window_pos(app,x,y);
		app_window_size(app,w,h);
	}

	// Inicializa contexto da thread do usuário (polymain)
	struct user_thread_context_t user_thread_context;
	user_thread_context.app_context=app_context;
	thread_signal_init(&user_thread_context.user_thread_initialized);
	thread_atomic_int_store(&user_thread_context.user_thread_finished,0);
	thread_signal_init(&user_thread_context.app_loop_finished);
	thread_signal_init(&user_thread_context.user_thread_terminated);

	// Cria a thread que executa polymain()
	thread_ptr_t user_thread=thread_create(user_thread_proc,&user_thread_context,THREAD_STACK_SIZE_DEFAULT);
	// Aguarda que a thread do usuário seja inicializada (timeout 5000ms)
	if(!thread_signal_wait(&user_thread_context.user_thread_initialized,5000)){
		// Erro: thread não inicializou no tempo esperado
		thread_signal_term(&user_thread_context.user_thread_initialized);
		thread_signal_term(&user_thread_context.app_loop_finished);
		thread_signal_term(&user_thread_context.user_thread_terminated);
		return EXIT_FAILURE;
	}

	// Cria emulador CRT (ou NULL se NULL_PLATFORM for definido)
	#ifdef NULL_PLATFORM
		crtemu_pc_t* crt=NULL;
	#else
		crtemu_pc_t* crt=crtemu_pc_create(NULL);
	#endif

	// Cria temporizador de frames com limite fixo de 60Hz
	// Garante que não executará mais rápido que 60FPS mesmo se vsync estiver desativado
	frametimer_t* frametimer=frametimer_create(NULL);
	frametimer_lock_rate(frametimer,60);
	signalvbl(); // Sinaliza o primeiro quadro

	// ========== LOOP PRINCIPAL ==========
	static APP_U32 screen_xbgr[sizeof(screen.buffer)]; // Buffer de pixels convertidos para XBGR
	int width=0;
	int height=0;
	int curs_vis=0; // Visibilidade do cursor
	int curs_x=0; // Posição X do cursor
	int curs_y=0; // Posição Y do cursor
	bool keystate[KEYCOUNT]={0}; // Estado atual das teclas
	enum keycode_t keys[256]={(enum keycode_t)0}; // Buffer de teclas pressionadas/liberadas
	char chars[256]={0}; // Buffer de caracteres digitados
	APP_U64 crt_time_us=0; // Tempo cumulativo para emulador CRT
	APP_U64 prev_time=app_time_count(app); // Tempo anterior para cálculo de delta

	// Loop executa enquanto a thread do usuário não terminou
	while(!thread_atomic_int_load(&user_thread_context.user_thread_finished)){
		// Processa eventos do sistema operacional
		app_state_t app_state=app_yield(app);

		// Atualiza temporizador de frames (limita a 60FPS)
		frametimer_update(frametimer);

		// Limpa buffers de entrada
		int keys_index=0;
		memset(keys,0,sizeof(keys));
		int chars_index=0;
		memset(chars,0,sizeof(chars));
		float relx=0;
		float rely=0;

		// Processa eventos de entrada (teclado e mouse)
		app_input_t input_handler=app_input(app);
		for(int i=0;i<input_handler.count;++i){
			app_input_event_t* event=&input_handler.events[i];

			if(event->type==APP_INPUT_KEY_DOWN){ // Tecla pressionada
				int index=(int)event->data.key;
				if(index>0&&index<KEYCOUNT){
					keystate[index]=true; // Atualiza estado persistente
					if(keys_index<255)keys[keys_index++]=(enum keycode_t)event->data.key; // Adiciona tecla pressionada ao buffer
				}

				// Trata F11 para alternar fullscreen
				if(event->data.key==APP_KEY_F11){
					fullscreen=!fullscreen;
					app_screenmode(app,fullscreen?APP_SCREENMODE_FULLSCREEN:APP_SCREENMODE_WINDOW);
					APP_U32 blank=0;
					app_pointer(app,1,1,&blank,0,0);
				}
			}
			else if(event->type==APP_INPUT_KEY_UP){ // Tecla liberada
				int index=(int)event->data.key;
				if(index>=0&&index<KEYCOUNT){
					keystate[index]=false; // Atualiza estado persistente
					if(keys_index<255)keys[keys_index++]=(enum keycode_t)(((uint32_t)event->data.key)|0x80000000); // Adiciona tecla liberada ao buffer (bit 31 setado para indicar KEY_UP)
				}
			}
			else if(event->type==APP_INPUT_CHAR) // Caractere digitado
				if(event->data.char_code>0)if(chars_index<255)chars[chars_index++]=event->data.char_code;
		}

		// Verifica se o botão de fechar foi clicado ou Alt+F4 pressionado
		if(app_state==APP_STATE_EXIT_REQUESTED){ // Sinaliza thread do usuário para sair
			thread_atomic_int_store(&exit_flag,1);
			signalvbl();
			break; // Sai do loop principal
		}

		// ========== CÓPIA DE DADOS DA THREAD DO USUÁRIO ==========
		// Lê dimensões atuais da tela
		width=screen.width;
		height=screen.height;
		uint8_t* poly_screen=screen.buffer;

		// Copia dados de vídeo da thread do usuário
		static uint8_t screen_handler[sizeof(screen.buffer)];
		memcpy(screen_handler,poly_screen,width*height);

		// Copia paleta de cores
		static uint32_t palette[256];
		memcpy(palette,screen.palette,1024);

		// Copia estado das teclas
		memcpy(input.keystate,keystate,sizeof(input.keystate));

		// ========== GERENCIAMENTO DE BUFFER DE TECLADO ==========
		// Alterna entre dois buffers para evitar condição de corrida
		enum keycode_t* poly_keybuffer;
		if(input.keybuffer==input.keybuffer0)poly_keybuffer=input.keybuffer1;
		else poly_keybuffer=input.keybuffer0;

		// Adiciona novas teclas ao buffer da thread do usuário
		enum keycode_t* keyin=keys;
		enum keycode_t* keyout=poly_keybuffer;
		enum keycode_t* keyend=poly_keybuffer+sizeof(input.keybuffer0)/sizeof(*input.keybuffer0)-1;
		while(*keyout&&keyout<keyend)++keyout; // Encontra fim do buffer existente
		while(*keyin){
			if(keyout>=keyend){ // Buffer cheio - move dados para cima e reinicia
				memmove(poly_keybuffer+1,poly_keybuffer,sizeof(input.keybuffer0)/sizeof(*input.keybuffer0)-1);
				--keyout;
			}
			*keyout++=*keyin++; // Copia nova tecla
		}
		*keyout=KEY_INVALID; // Marca fim do buffer

		// ========== GERENCIAMENTO DE BUFFER DE CARACTERES ==========
		// Similar ao de teclado, mas para caracteres ASCII
		char* poly_charbuffer;
		if(input.charbuffer==input.charbuffer0)poly_charbuffer=input.charbuffer1;
		else poly_charbuffer=input.charbuffer0;
		char* charin=chars;
		char* charout=poly_charbuffer;
		char* charend=poly_charbuffer+sizeof(input.charbuffer0)/sizeof(*input.charbuffer0)-1;
		while(*charout&&charout<charend)++charout; // Encontra fim do buffer existente
		while(*charin){
			if(charout>=charend){ // Buffer cheio - move dados para cima
				memmove(poly_charbuffer+1,poly_charbuffer,sizeof(input.charbuffer0)/sizeof(*input.charbuffer0)-1);
				--charout;
			}
			*charout++=*charin++; // Copia novo caractere
		}
		*charout='\0'; // Termina string com null

		// Sinaliza thread do usuário que o frame foi processado
		signalvbl();

		// ========== RENDERIZAÇÃO ==========
		// Converte paleta indexada (8-bit) para cores XBGR (32-bit)
		for(int y=0;y<height;++y)
			for(int x=0;x<width;++x)
				screen_xbgr[x+y*width]=palette[screen_handler[x+y*width]];

		// Calcula delta de tempo desde o último frame (em microsegundos)
		APP_U64 time=app_time_count(app);
		APP_U64 freq=app_time_freq(app);
		APP_U64 delta_time_us=(time-prev_time)/((freq>1000000?freq/1000000:1));
		prev_time=time;

		// Acumula tempo para emulador CRT
		crt_time_us+=delta_time_us;

		// Renderiza com efeito de CRT (se habilitado)
		if(crt){
			#ifndef DISABLE_SCREEN_FRAME
				// Renderiza com moldura de monitor CRT
				crtemu_pc_present(crt,crt_time_us,screen_xbgr,width,height,0xffffff,0xff1a1a1a);
			#else
				// Renderiza sem moldura
				crtemu_pc_present(crt,crt_time_us,screen_xbgr,width,height,0xffffff,0xff000000);
			#endif
		}
		// Apresenta resultado na tela
		app_present(app,NULL,1,1,0xffffff,0xff1a1a1a);
	}

	// ========== FINALIZANDO ==========
	// Sinaliza thread do usuário que pode terminar
	thread_signal_raise(&user_thread_context.app_loop_finished);

	// Aguarda término da thread do usuário (timeout 170ms)
	int user_exit=thread_signal_wait(&user_thread_context.user_thread_terminated,170);
	if(!user_exit){ // Se thread não terminou, faz fade-out (30 frames de escurecimento)
		for(int i=0;i<60;++i){
			APP_U64 time=app_time_count(app);
			APP_U64 delta_time_us=(time-prev_time)/(app_time_freq(app)/1000000);
			prev_time=time;
			crt_time_us+=delta_time_us;

			// Calcula cor de fade (vai de branco para preto)
			int v=((60-i)*255)/60;
			uint32_t fade=(v<<16)|v<<8|v;

			// Renderiza com efeito de fade
			if(crt)crtemu_pc_present(crt,crt_time_us,screen_xbgr,width,height,fade,0xff1a1a1a);
			app_present(app,NULL,1,1,0xffffff,0xff1a1a1a);
			frametimer_update(frametimer);
		}

		// Tenta novamente aguardar thread (timeout 30ms)
		user_exit=thread_signal_wait(&user_thread_context.user_thread_terminated,30);
		if(!user_exit)exit(EXIT_FAILURE); // Falha se thread não terminou mesmo após fade-out
	}

	// Libera recursos de sincronização
	thread_signal_term(&user_thread_context.user_thread_initialized);
	thread_signal_term(&user_thread_context.app_loop_finished);
	thread_signal_term(&user_thread_context.user_thread_terminated);

	// Libera recursos de renderização
	frametimer_destroy(frametimer);
	if(crt)crtemu_pc_destroy(crt);

	// Aguarda thread do usuário e retorna seu código de saída
	return thread_join(user_thread);
}

	#define APP_IMPLEMENTATION
	#ifdef NULL_PLATFORM
		#define APP_NULL
	#elif defined(_WIN32)
		#define APP_WINDOWS
	#else
		#define APP_SDL
	#endif

	#define APP_LOG(ctx,level,message)
	#include "app.h"

	#define CRTEMU_PC_IMPLEMENTATION
	#include "crtemu_pc.h"

	#define FRAMETIMER_IMPLEMENTATION
	#include "frametimer.h"

	#define THREAD_IMPLEMENTATION
	#include "thread.h"

int main(int argc,char** argv){
	(void)argc,(void)argv;
	struct app_context_t app_context;
	app_context.argc=argc;
	app_context.argv=argv;
	return app_run(app_proc,&app_context,NULL,NULL,NULL);
}

	#ifdef _WIN32
	//pass-through so the program will build with either /SUBSYSTEM:WINDOWS or /SUBSYSTEM:CONSOLE
	int WINAPI __stdcall WinMain(HINSTANCE a,HINSTANCE b,char* c,int d){
		(void)a,b,c,d;
		return main(__argc,__argv);
	}
	#endif

#endif /* POLY */

#ifndef NO_MAIN_DEF
	#define main polymain
#endif
