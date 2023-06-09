#include <stdio.h>
#include <string.h>
#include <sys/iosupport.h>
#include <3ds/gfx.h>
#include <3ds/console.h>
#include <3ds/svc.h>

#include "default_font_bin.h"

//set up the palette for color printing
static u16 colorTable[] = {
	RGB8_to_565(  0,  0,  0),	// black
	RGB8_to_565(128,  0,  0),	// red
	RGB8_to_565(  0,128,  0),	// green
	RGB8_to_565(128,128,  0),	// yellow
	RGB8_to_565(  0,  0,128),	// blue
	RGB8_to_565(128,  0,128),	// magenta
	RGB8_to_565(  0,128,128),	// cyan
	RGB8_to_565(192,192,192),	// white

	RGB8_to_565(128,128,128),	// bright black
	RGB8_to_565(255,  0,  0),	// bright red
	RGB8_to_565(  0,255,  0),	// bright green
	RGB8_to_565(255,255,  0),	// bright yellow
	RGB8_to_565(  0,  0,255),	// bright blue
	RGB8_to_565(255,  0,255),	// bright magenta
	RGB8_to_565(  0,255,255),	// bright cyan
	RGB8_to_565(255,255,255),	// bright white

	RGB8_to_565(  0,  0,  0),	// faint black
	RGB8_to_565( 64,  0,  0),	// faint red
	RGB8_to_565(  0, 64,  0),	// faint green
	RGB8_to_565( 64, 64,  0),	// faint yellow
	RGB8_to_565(  0,  0, 64),	// faint blue
	RGB8_to_565( 64,  0, 64),	// faint magenta
	RGB8_to_565(  0, 64, 64),	// faint cyan
	RGB8_to_565( 96, 96, 96),	// faint white
};

static const u8 colorCube[] = {
	0x00, 0x5f, 0x87, 0xaf, 0xd7, 0xff,
};

static const u8 grayScale[] = {
	0x08, 0x12, 0x1c, 0x26, 0x30, 0x3a, 0x44, 0x4e,
	0x58, 0x62, 0x6c, 0x76, 0x80, 0x8a, 0x94, 0x9e,
	0xa8, 0xb2, 0xbc, 0xc6, 0xd0, 0xda, 0xe4, 0xee,
};

PrintConsole defaultConsole =
{
	//Font:
	{
		(u8*)default_font_bin, //font gfx
		0, //first ascii character in the set
		256 //number of characters in the font set
	},
	(u16*)NULL,
	0,0,	//cursorX cursorY
	0,0,	//prevcursorX prevcursorY
	40,		//console width
	30,		//console height
	0,		//window x
	0,		//window y
	40,		//window width
	30,		//window height
	3,		//tab size
	7,		// foreground color
	0,		// background color
	0,		// flags
	0,		//print callback
	false	//console initialized
};

static bool parseColor (char **esc, int *escLen, u16 *color, bool *custom)
{
	unsigned int p;
	unsigned int n;
	unsigned int r;
	unsigned int g;
	unsigned int b;
	int consumed;

	if (sscanf (*esc, "%d;%n", &p, &consumed) != 1)
		return false;

	*esc    += consumed;
	*escLen -= consumed;

	if (p == 5) {
		if (sscanf (*esc, "%u%n", &n, &consumed) != 1)
			return false;

		*esc    += consumed;
		*escLen -= consumed;

		if (n <= 15) {
			*color  = n;
			*custom = false;
		} else if (n <= 231) {
			n -= 16;
			r = n / 36;
			g = (n - r * 36) / 6;
			b = n - r * 36 - g * 6;

			*color  = RGB8_to_565 (colorCube[r], colorCube[g], colorCube[b]);
			*custom = true;
		} else if (n <= 255) {
			n -= 232;

			*color  = RGB8_to_565 (grayScale[n], grayScale[n], grayScale[n]);
			*custom = true;
		} else {
			return false;
		}

		return true;
	} else if (p == 2) {
		if (sscanf (*esc, "%u;%u;%u%n", &r, &g, &b, &consumed) != 3)
			return false;

		*esc    += consumed;
		*escLen -= consumed;

		*color  = RGB8_to_565 (r, g, b);
		*custom = true;

		return true;
	}

	return false;
}

PrintConsole currentCopy;

PrintConsole* currentConsole = &currentCopy;

PrintConsole* consoleGetDefault(void){return &defaultConsole;}

void consolePrintChar(int c);
void consoleDrawChar(int c);

//---------------------------------------------------------------------------------
static void consoleCls(char mode) {
//---------------------------------------------------------------------------------

	int i = 0;
	int colTemp,rowTemp;

	switch (mode)
	{
	case '[':
	case '0':
		{
			colTemp = currentConsole->cursorX ;
			rowTemp = currentConsole->cursorY ;

			while(i++ < ((currentConsole->windowHeight * currentConsole->windowWidth) - (rowTemp * currentConsole->consoleWidth + colTemp)))
				consolePrintChar(' ');

			currentConsole->cursorX  = colTemp;
			currentConsole->cursorY  = rowTemp;
			break;
		}
	case '1':
		{
			colTemp = currentConsole->cursorX ;
			rowTemp = currentConsole->cursorY ;

			currentConsole->cursorY  = 0;
			currentConsole->cursorX  = 0;

			while (i++ < (rowTemp * currentConsole->windowWidth + colTemp))
				consolePrintChar(' ');

			currentConsole->cursorX  = colTemp;
			currentConsole->cursorY  = rowTemp;
			break;
		}
	case '2':
		{
			currentConsole->cursorY  = 0;
			currentConsole->cursorX  = 0;

			while(i++ < currentConsole->windowHeight * currentConsole->windowWidth)
				consolePrintChar(' ');

			currentConsole->cursorY  = 0;
			currentConsole->cursorX  = 0;
			break;
		}
	}
	gfxFlushBuffers();
}
//---------------------------------------------------------------------------------
static void consoleClearLine(char mode) {
//---------------------------------------------------------------------------------

	int i = 0;
	int colTemp;

	switch (mode)
	{
	case '[':
	case '0':
		{
			colTemp = currentConsole->cursorX ;

			while(i++ < (currentConsole->windowWidth - colTemp)) {
				consolePrintChar(' ');
			}

			currentConsole->cursorX  = colTemp;

			break;
		}
	case '1':
		{
			colTemp = currentConsole->cursorX ;

			currentConsole->cursorX  = 0;

			while(i++ < ((currentConsole->windowWidth - colTemp)-2)) {
				consolePrintChar(' ');
			}

			currentConsole->cursorX  = colTemp;

			break;
		}
	case '2':
		{
			colTemp = currentConsole->cursorX ;

			currentConsole->cursorX  = 0;

			while(i++ < currentConsole->windowWidth) {
				consolePrintChar(' ');
			}

			currentConsole->cursorX  = colTemp;

			break;
		}
	}
	gfxFlushBuffers();
}


//---------------------------------------------------------------------------------
static inline void consolePosition(int x, int y) {
//---------------------------------------------------------------------------------
	// invalid position
	if(x < 0 || y < 0)
		return;

	// 1-based, but we'll take a 0
	if(x < 1)
		x = 1;
	if(y < 1)
		y = 1;

	// clip to console edge
	if(x > currentConsole->windowWidth)
		x = currentConsole->windowWidth;
	if(y > currentConsole->windowHeight)
		y = currentConsole->windowHeight;

	// 1-based adjustment
	currentConsole->cursorX = x - 1;
	currentConsole->cursorY = y - 1;
}

//---------------------------------------------------------------------------------
ssize_t con_write(struct _reent *r,void *fd,const char *ptr, size_t len) {
//---------------------------------------------------------------------------------

	char chr;

	int i, count = 0;
	char *tmp = (char*)ptr;

	if(!tmp) return -1;

	i = 0;

	while(i<len) {

		chr = *(tmp++);
		i++; count++;

		if ( chr == 0x1b && len > 1 && *tmp == '[' ) {
			bool escaping = true;
			char *escapeseq	= tmp++;
			int escapelen = 1;
			i++; count++;

			do {
				chr = *(tmp++);
				i++; count++; escapelen++;
				int parameter, assigned, consumed;

				// make sure parameters are positive values and delimited by semicolon
				if((chr >= '0' && chr <= '9') || chr == ';')
					continue;

				switch (chr) {
					//---------------------------------------
					// Cursor directional movement
					//---------------------------------------
					case 'A':
						consumed = 0;
						assigned = sscanf(escapeseq,"[%dA%n", &parameter, &consumed);
						if (assigned==0) parameter = 1;
						if (consumed)
							currentConsole->cursorY  =  (currentConsole->cursorY  - parameter) < 0 ? 0 : currentConsole->cursorY  - parameter;
						escaping = false;
						break;
					case 'B':
						consumed = 0;
						assigned = sscanf(escapeseq,"[%dB%n", &parameter, &consumed);
						if (assigned==0) parameter = 1;
						if (consumed)
							currentConsole->cursorY  =  (currentConsole->cursorY  + parameter) > currentConsole->windowHeight - 1 ? currentConsole->windowHeight - 1 : currentConsole->cursorY  + parameter;
						escaping = false;
						break;
					case 'C':
						consumed = 0;
						assigned = sscanf(escapeseq,"[%dC%n", &parameter, &consumed);
						if (assigned==0) parameter = 1;
						if (consumed)
							currentConsole->cursorX  =  (currentConsole->cursorX  + parameter) > currentConsole->windowWidth - 1 ? currentConsole->windowWidth - 1 : currentConsole->cursorX  + parameter;
						escaping = false;
						break;
					case 'D':
						consumed = 0;
						assigned = sscanf(escapeseq,"[%dD%n", &parameter, &consumed);
						if (assigned==0) parameter = 1;
						if (consumed)
							currentConsole->cursorX  =  (currentConsole->cursorX  - parameter) < 0 ? 0 : currentConsole->cursorX  - parameter;
						escaping = false;
						break;
					//---------------------------------------
					// Cursor position movement
					//---------------------------------------
					case 'H':
					case 'f':
					{
						int  x, y;
						char c;
						if(sscanf(escapeseq,"[%d;%d%c", &y, &x, &c) == 3 && (c == 'f' || c == 'H')) {
							consolePosition(x, y);
							escaping = false;
							break;
						}

						x = y = 1;
						if(sscanf(escapeseq,"[%d;%c", &y, &c) == 2 && (c == 'f' || c == 'H')) {
							consolePosition(x, y);
							escaping = false;
							break;
						}

						x = y = 1;
						if(sscanf(escapeseq,"[;%d%c", &x, &c) == 2 && (c == 'f' || c == 'H')) {
							consolePosition(x, y);
							escaping = false;
							break;
						}

						x = y = 1;
						if(sscanf(escapeseq,"[;%c", &c) == 1 && (c == 'f' || c == 'H')) {
							consolePosition(x, y);
							escaping = false;
							break;
						}

						// invalid format
						escaping = false;
						break;
					}
					//---------------------------------------
					// Screen clear
					//---------------------------------------
					case 'J':
						if(escapelen <= 3)
							consoleCls(escapeseq[escapelen-2]);
						escaping = false;
						break;
					//---------------------------------------
					// Line clear
					//---------------------------------------
					case 'K':
						if(escapelen <= 3)
							consoleClearLine(escapeseq[escapelen-2]);
						escaping = false;
						break;
					//---------------------------------------
					// Save cursor position
					//---------------------------------------
					case 's':
						if(escapelen == 2) {
							currentConsole->prevCursorX  = currentConsole->cursorX ;
							currentConsole->prevCursorY  = currentConsole->cursorY ;
						}
						escaping = false;
						break;
					//---------------------------------------
					// Load cursor position
					//---------------------------------------
					case 'u':
						if(escapelen == 2) {
							currentConsole->cursorX  = currentConsole->prevCursorX ;
							currentConsole->cursorY  = currentConsole->prevCursorY ;
						}
						escaping = false;
						break;
					//---------------------------------------
					// Color scan codes
					//---------------------------------------
					case 'm':
						escapeseq++;
						escapelen--;

						do {
							bool custom;
							parameter = 0;
							if (escapelen == 1) {
								consumed = 1;
							} else if (memchr(escapeseq,';',escapelen)) {
								sscanf(escapeseq,"%d;%n", &parameter, &consumed);
							} else {
								sscanf(escapeseq,"%dm%n", &parameter, &consumed);
							}

							escapeseq += consumed;
							escapelen -= consumed;

							switch(parameter) {
							case 0: // reset
								currentConsole->flags = 0;
								currentConsole->bg    = 0;
								currentConsole->fg    = 7;
								break;

							case 1: // bold
								currentConsole->flags &= ~CONSOLE_COLOR_FAINT;
								currentConsole->flags |= CONSOLE_COLOR_BOLD;
								break;

							case 2: // faint
								currentConsole->flags &= ~CONSOLE_COLOR_BOLD;
								currentConsole->flags |= CONSOLE_COLOR_FAINT;
								break;

							case 3: // italic
								currentConsole->flags |= CONSOLE_ITALIC;
								break;

							case 4: // underline
								currentConsole->flags |= CONSOLE_UNDERLINE;
								break;

							case 5: // blink slow
								currentConsole->flags &= ~CONSOLE_BLINK_FAST;
								currentConsole->flags |= CONSOLE_BLINK_SLOW;
								break;

							case 6: // blink fast
								currentConsole->flags &= ~CONSOLE_BLINK_SLOW;
								currentConsole->flags |= CONSOLE_BLINK_FAST;
								break;

							case 7: // reverse video
								currentConsole->flags |= CONSOLE_COLOR_REVERSE;
								break;

							case 8: // conceal
								currentConsole->flags |= CONSOLE_CONCEAL;
								break;

							case 9: // crossed-out
								currentConsole->flags |= CONSOLE_CROSSED_OUT;
								break;

							case 21: // bold off
								currentConsole->flags &= ~CONSOLE_COLOR_BOLD;
								break;

							case 22: // normal color
								currentConsole->flags &= ~CONSOLE_COLOR_BOLD;
								currentConsole->flags &= ~CONSOLE_COLOR_FAINT;
								break;

							case 23: // italic off
								currentConsole->flags &= ~CONSOLE_ITALIC;
								break;

							case 24: // underline off
								currentConsole->flags &= ~CONSOLE_UNDERLINE;
								break;

							case 25: // blink off
								currentConsole->flags &= ~CONSOLE_BLINK_SLOW;
								currentConsole->flags &= ~CONSOLE_BLINK_FAST;
								break;

							case 27: // reverse off
								currentConsole->flags &= ~CONSOLE_COLOR_REVERSE;
								break;

							case 29: // crossed-out off
								currentConsole->flags &= ~CONSOLE_CROSSED_OUT;
								break;

							case 30 ... 37: // writing color
								currentConsole->flags &= ~CONSOLE_FG_CUSTOM;
								currentConsole->fg     = parameter - 30;
								break;

							case 38: // custom foreground color
								if (parseColor (&escapeseq, &escapelen, &currentConsole->fg, &custom)) {
									if (custom)
										currentConsole->flags |= CONSOLE_FG_CUSTOM;
									else
										currentConsole->flags &= ~CONSOLE_FG_CUSTOM;

									if (!custom && currentConsole->fg < 16) {
										currentConsole->flags &= ~CONSOLE_COLOR_FAINT;
										if (currentConsole->fg < 8)
											currentConsole->flags &= ~CONSOLE_COLOR_BOLD;
										else
											currentConsole->flags |= CONSOLE_COLOR_BOLD;
									}

									// consume next ; or m
									++escapeseq;
									--escapelen;
								} else {
									// stop processing
									escapelen = 0;
								}
								break;

							case 39: // reset foreground color
								currentConsole->flags &= ~CONSOLE_FG_CUSTOM;
								currentConsole->fg     = 7;
								break;

							case 40 ... 47: // screen color
								currentConsole->flags &= ~CONSOLE_BG_CUSTOM;
								currentConsole->bg = parameter - 40;
								break;

							case 48: // custom background color
								if (parseColor (&escapeseq, &escapelen, &currentConsole->bg, &custom)) {
									if (custom)
										currentConsole->flags |= CONSOLE_BG_CUSTOM;
									else
										currentConsole->flags &= ~CONSOLE_BG_CUSTOM;

									// consume next ; or m
									++escapeseq;
									--escapelen;
								} else {
									// stop processing
									escapelen = 0;
								}
								break;

							case 49: // reset background color
								currentConsole->flags &= ~CONSOLE_BG_CUSTOM;
								currentConsole->fg = 0;
								break;
							}
						} while (escapelen > 0);

						escaping = false;
						break;

					default:
						// some sort of unsupported escape; just gloss over it
						escaping = false;
						break;
				}
			} while (escaping);
			continue;
		}

		consolePrintChar(chr);
	}

	return count;
}

static const devoptab_t dotab_stdout = {
	"con",
	0,
	NULL,
	NULL,
	con_write,
	NULL,
	NULL,
	NULL
};

//---------------------------------------------------------------------------------
static ssize_t debug_write(struct _reent *r, void *fd, const char *ptr, size_t len) {
//---------------------------------------------------------------------------------
	svcOutputDebugString(ptr,len);
	return len;
}

static const devoptab_t dotab_svc = {
	"svc",
	0,
	NULL,
	NULL,
	debug_write,
	NULL,
	NULL,
	NULL
};


static const devoptab_t dotab_null = {
	"null",
	0,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

//---------------------------------------------------------------------------------
PrintConsole* consoleInit(gfxScreen_t screen, PrintConsole* console) {
//---------------------------------------------------------------------------------

	static bool firstConsoleInit = true;

	if(firstConsoleInit) {
		devoptab_list[STD_OUT] = &dotab_stdout;
		devoptab_list[STD_ERR] = &dotab_stdout;

		setvbuf(stdout, NULL , _IONBF, 0);
		setvbuf(stderr, NULL , _IONBF, 0);

		firstConsoleInit = false;
	}

	if(console) {
		currentConsole = console;
	} else {
		console = currentConsole;
	}

	*currentConsole = defaultConsole;

	console->consoleInitialised = 1;

	gfxSetScreenFormat(screen,GSP_RGB565_OES);
	gfxSetDoubleBuffering(screen,false);
	gfxSwapBuffersGpu();
	gspWaitForVBlank();

	console->frameBuffer = (u16*)gfxGetFramebuffer(screen, GFX_LEFT, NULL, NULL);

	if(screen==GFX_TOP) {
		bool isWide = gfxIsWide();
		console->consoleWidth = isWide ? 100 : 50;
		console->windowWidth = isWide ? 100 : 50;
	}

	consoleCls('2');

	return currentConsole;

}

//---------------------------------------------------------------------------------
void consoleDebugInit(debugDevice device){
//---------------------------------------------------------------------------------

	int buffertype = _IONBF;

	switch(device) {

	case debugDevice_SVC:
		devoptab_list[STD_ERR] = &dotab_svc;
		buffertype = _IOLBF;
		break;
	case debugDevice_CONSOLE:
		devoptab_list[STD_ERR] = &dotab_stdout;
		break;
	case debugDevice_NULL:
		devoptab_list[STD_ERR] = &dotab_null;
		break;
	}
	setvbuf(stderr, NULL , buffertype, 0);

}

//---------------------------------------------------------------------------------
PrintConsole *consoleSelect(PrintConsole* console){
//---------------------------------------------------------------------------------
	PrintConsole *tmp = currentConsole;
	currentConsole = console;
	return tmp;
}

//---------------------------------------------------------------------------------
void consoleSetFont(PrintConsole* console, ConsoleFont* font){
//---------------------------------------------------------------------------------

	if(!console) console = currentConsole;

	console->font = *font;

}

//---------------------------------------------------------------------------------
static void newRow() {
//---------------------------------------------------------------------------------


	currentConsole->cursorY ++;


	if(currentConsole->cursorY  >= currentConsole->windowHeight)  {
		currentConsole->cursorY --;
		u16 *dst = &currentConsole->frameBuffer[(currentConsole->windowX * 8 * 240) + (239 - (currentConsole->windowY * 8))];
		u16 *src = dst - 8;

		int i,j;

		for (i=0; i<currentConsole->windowWidth*8; i++) {
			u32 *from = (u32*)((int)src & ~3);
			u32 *to = (u32*)((int)dst & ~3);
			for (j=0;j<(((currentConsole->windowHeight-1)*8)/2);j++) *(to--) = *(from--);
			dst += 240;
			src += 240;
		}

		consoleClearLine('2');
	}
}
//---------------------------------------------------------------------------------
void consoleDrawChar(int c) {
//---------------------------------------------------------------------------------
	c -= currentConsole->font.asciiOffset;
	if ( c < 0 || c > currentConsole->font.numChars ) return;

	u8 *fontdata = currentConsole->font.gfx + (8 * c);

	u16 fg = currentConsole->fg;
	u16 bg = currentConsole->bg;

	if (!(currentConsole->flags & CONSOLE_FG_CUSTOM)) {
		if (currentConsole->flags & CONSOLE_COLOR_BOLD) {
			fg = colorTable[fg + 8];
		} else if (currentConsole->flags & CONSOLE_COLOR_FAINT) {
			fg = colorTable[fg + 16];
		} else {
			fg = colorTable[fg];
		}
	}

	if (!(currentConsole->flags & CONSOLE_BG_CUSTOM)) {
		bg = colorTable[bg];
	}

	if (currentConsole->flags & CONSOLE_COLOR_REVERSE) {
		u16 tmp = fg;
		fg = bg;
		bg = tmp;
	}

	u8 b1 = *(fontdata++);
	u8 b2 = *(fontdata++);
	u8 b3 = *(fontdata++);
	u8 b4 = *(fontdata++);
	u8 b5 = *(fontdata++);
	u8 b6 = *(fontdata++);
	u8 b7 = *(fontdata++);
	u8 b8 = *(fontdata++);

	if (currentConsole->flags & CONSOLE_UNDERLINE) b8 = 0xff;

	if (currentConsole->flags & CONSOLE_CROSSED_OUT) b4 = 0xff;

	u8 mask = 0x80;


	int i;

	int x = (currentConsole->cursorX + currentConsole->windowX) * 8;
	int y = ((currentConsole->cursorY + currentConsole->windowY) *8 );

	u16 *screen = &currentConsole->frameBuffer[(x * 240) + (239 - (y + 7))];

	for (i=0;i<8;i++) {
		if (b8 & mask) { *(screen++) = fg; }else{ *(screen++) = bg; }
		if (b7 & mask) { *(screen++) = fg; }else{ *(screen++) = bg; }
		if (b6 & mask) { *(screen++) = fg; }else{ *(screen++) = bg; }
		if (b5 & mask) { *(screen++) = fg; }else{ *(screen++) = bg; }
		if (b4 & mask) { *(screen++) = fg; }else{ *(screen++) = bg; }
		if (b3 & mask) { *(screen++) = fg; }else{ *(screen++) = bg; }
		if (b2 & mask) { *(screen++) = fg; }else{ *(screen++) = bg; }
		if (b1 & mask) { *(screen++) = fg; }else{ *(screen++) = bg; }
		mask >>= 1;
		screen += 240 - 8;
	}

}

//---------------------------------------------------------------------------------
void consolePrintChar(int c) {
//---------------------------------------------------------------------------------
	if (c==0) return;

	if(currentConsole->PrintChar)
		if(currentConsole->PrintChar(currentConsole, c))
			return;

	if(currentConsole->cursorX  >= currentConsole->windowWidth) {
		currentConsole->cursorX  = 0;

		newRow();
	}

	switch(c) {
		/*
		The only special characters we will handle are tab (\t), carriage return (\r), line feed (\n)
		and backspace (\b).
		Carriage return & line feed will function the same: go to next line and put cursor at the beginning.
		For everything else, use VT sequences.

		Reason: VT sequences are more specific to the task of cursor placement.
		The special escape sequences \b \f & \v are archaic and non-portable.
		*/
		case 8:
			currentConsole->cursorX--;

			if(currentConsole->cursorX < 0) {
				if(currentConsole->cursorY > 0) {
					currentConsole->cursorX = currentConsole->windowX - 1;
					currentConsole->cursorY--;
				} else {
					currentConsole->cursorX = 0;
				}
			}

			consoleDrawChar(' ');
			break;

		case 9:
			currentConsole->cursorX  += currentConsole->tabSize - ((currentConsole->cursorX)%(currentConsole->tabSize));
			break;
		case 10:
			newRow();
		case 13:
			currentConsole->cursorX  = 0;
			gfxFlushBuffers();
			break;
		default:
			consoleDrawChar(c);
			++currentConsole->cursorX ;
			break;
	}
}

//---------------------------------------------------------------------------------
void consoleClear(void) {
//---------------------------------------------------------------------------------
	consoleCls('2');
}

//---------------------------------------------------------------------------------
void consoleSetWindow(PrintConsole* console, int x, int y, int width, int height){
//---------------------------------------------------------------------------------

	if(!console) console = currentConsole;

	console->windowWidth = width;
	console->windowHeight = height;
	console->windowX = x;
	console->windowY = y;

	console->cursorX = 0;
	console->cursorY = 0;

}


