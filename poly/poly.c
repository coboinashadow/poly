#define POLY
#include "poly.h"

int getpixel(int x,int y){
	if(x>=0&&y>=0&&x<draw.width&&y<draw.height)return draw.buffer[x+draw.width*y];
	else return 0;
}

void putpixel(int x,int y,int color){
	if(x>=0&&y>=0&&x<draw.width&&y<draw.height)draw.buffer[x+draw.width*y]=(uint8_t)color;
}

void setcolor(int color){
	if(color>=0&&color<=255)screen.color=color;
}

int getcolor(){
	return screen.color;
}

void hline(int x,int y,int len,int color){
	if(y<0||y>=draw.height)return;
	if(x<0){
		len+=x;
		x=0;
	}
	if(x+len>draw.width)len=draw.width-x;
	uint8_t* scr=draw.buffer+y*draw.width+x;
	uint8_t* end=scr+len;
	while(scr<end)*scr++=(uint8_t)color;
}

void line(int x1,int y1,int x2,int y2){
	int color=screen.color;
	int dx=x2-x1;
	dx=dx<0?-dx:dx;
	int sx=x1<x2?1:-1;
	int dy=y2-y1;
	dy=dy<0?-dy:dy;
	int sy=y1<y2?1:-1;
	int err=(dx>dy?dx:-dy)/2;
	int x=x1;
	int y=y1;
	while(x!=x2||y!=y2){
		putpixel(x,y,color);
		int e2=err;
		if(e2>-dx){
			err-=dy;
			x+=sx;
		}
		if(e2<dy){
			err+=dx;
			y+=sy;
		}
	}
	putpixel(x,y,color);
}

void rectangle(int x,int y,int w,int h){
	int color=screen.color;
	hline(x,y,w,color);
	hline(x,y+h,w,color);
	line(x,y,x,y+h);
	line(x+w-1,y,x+w-1,y+h);
}

void bar(int x,int y,int w,int h){
	int color=screen.color;
	for(int i=y;i<y+h;++i){
		hline(x,i,w,color);
	}
}

void circle(int x,int y,int r){
	int color=screen.color;
	int f=1-r;
	int dx=0;
	int dy=-2*r;
	int ix=0;
	int iy=r;
	putpixel(x,y+r,color);
	putpixel(x,y-r,color);
	putpixel(x+r,y,color);
	putpixel(x-r,y,color);
	while(ix<iy){
		if(f>=0){
			--iy;
			dy+=2;
			f+=dy;
		}
		++ix;
		dx+=2;
		f+=dx+1;
		putpixel(x+ix,y+iy,color);
		putpixel(x-ix,y+iy,color);
		putpixel(x+ix,y-iy,color);
		putpixel(x-ix,y-iy,color);
		putpixel(x+iy,y+ix,color);
		putpixel(x-iy,y+ix,color);
		putpixel(x+iy,y-ix,color);
		putpixel(x-iy,y-ix,color);
	}
}

void fillcircle(int x,int y,int r){
	int color=screen.color;
	int f=1-r;
	int dx=0;
	int dy=-2*r;
	int ix=0;
	int iy=r;
	while(ix<=iy){
		hline(x-iy,y+ix,2*iy,color);
		hline(x-iy,y-ix,2*iy,color);
		if(f>=0){
			hline(x-ix,y+iy,2*ix,color);
			hline(x-ix,y-iy,2*ix,color);
			--iy;
			dy+=2;
			f+=dy;
		}
		++ix;
		dx+=2;
		f+=dx+1;
	}
}


void ellipse(int x,int y,int rx,int ry){
	int color=screen.color;
	int asq=rx*rx;
	int bsq=ry*ry;
	putpixel(x,y+ry,color);
	putpixel(x,y-ry,color);
	int wx=0;
	int wy=ry;
	int xa=0;
	int ya=asq*2*ry;
	int thresh=asq/4-asq*ry;
	for(;;){
		thresh+=xa+bsq;
		if(thresh>=0){
			ya-=asq*2;
			thresh-=ya;
			--wy;
		}
		xa+=bsq*2;
		++wx;
		if(xa>=ya){
			break;
		}
		putpixel(x+wx,y-wy,color);
		putpixel(x-wx,y-wy,color);
		putpixel(x+wx,y+wy,color);
		putpixel(x-wx,y+wy,color);
	}
	putpixel(x+rx,y,color);
	putpixel(x-rx,y,color);
	wx=rx;
	wy=0;
	xa=bsq*2*rx;
	ya=0;
	thresh=bsq/4-bsq*rx;
	for(;;){
		thresh+=ya+asq;
		if(thresh>=0){
			xa-=bsq*2;
			thresh=thresh-xa;
			--wx;
		}
		ya+=asq*2;
		++wy;
		if(ya>xa){
			break;
		}
		putpixel(x+wx,y-wy,color);
		putpixel(x-wx,y-wy,color);
		putpixel(x+wx,y+wy,color);
		putpixel(x-wx,y+wy,color);
	}
}


void fillellipse(int x,int y,int rx,int ry){
	int color=screen.color;
	int asq=rx*rx;
	int bsq=ry*ry;
	int wx=0;
	int wy=ry;
	int xa=0;
	int ya=asq*2*ry;
	int thresh=asq/4-asq*ry;
	for(;;){
		thresh+=xa+bsq;
		if(thresh>=0){
			ya-=asq*2;
			thresh-=ya;
			hline(x-wx,y-wy,wx*2,color);
			hline(x-wx,y+wy,wx*2,color);
			--wy;
		}
		xa+=bsq*2;
		++wx;
		if(xa>=ya){
			break;
		}
	}
	hline(x-rx,y,rx*2,color);
	wx=rx;
	wy=0;
	xa=bsq*2*rx;
	ya=0;
	thresh=bsq/4-bsq*rx;
	for(;;){
		thresh+=ya+asq;
		if(thresh>=0){
			xa-=bsq*2;
			thresh=thresh-xa;
			--wx;
		}
		ya+=asq*2;
		++wy;
		if(ya>xa){
			break;
		}
		hline(x-wx,y-wy,wx*2,color);
		hline(x-wx,y+wy,wx*2,color);
	}
}

void drawpoly(int* points_xy,int count){
	for(int i=0;i<count-1;++i){
		line(points_xy[i*2+0],points_xy[i*2+1],
			points_xy[(i+1)* 2+0],points_xy[(i+1)* 2+1]);
	}
}

void fillpoly(int* points_xy,int count){
	#define MAX_POLYGON_POINTS 256
	static int node_x[MAX_POLYGON_POINTS];

	if(count<=0||count>MAX_POLYGON_POINTS){
		return;
	}
	int color=screen.color;
	int min_y=points_xy[0+1];
	int max_y=min_y;
	//find extents
	for(int i=1;i<count;++i){
		if(points_xy[i*2+1]<min_y)min_y=points_xy[i*2+1];
		if(points_xy[i*2+1]>max_y)max_y=points_xy[i*2+1];
	}
	for(int y=min_y;y<max_y;++y){
		//find intersection points_xy
		int nodes=0;
		int j=count-1;
		for(int i=0;i<count;++i){
			if((points_xy[i*2+1]<=y&&points_xy[j*2+1]>y)||
				(points_xy[j*2+1]<=y&&points_xy[i*2+1]>y)){

				int dx=points_xy[j*2+0]-points_xy[i*2+0];
				int dy=points_xy[j*2+1]-points_xy[i*2+1];
				node_x[nodes++]=points_xy[i*2+0]+((y-points_xy[i*2+1])* dx)/dy ;
			}
			j=i;
		}
		//sort by x
		int xi=0;
		while(xi<nodes-1){
			if(node_x[xi]>node_x[xi+1]){
				int swap=node_x[xi];
				node_x[xi]=node_x[xi+1];
				node_x[xi+1]=swap;
				if(xi)--xi;
			} else{
				++xi;
			}
		}
		for(int i=0;i<nodes;i+=2){
			hline(node_x[i],y,node_x[i+1]-node_x[i],color);
		}
	}
}

void traco1(int x,int y){line(x+1,y+0,x+3,y+0);}
void traco2(int x,int y){line(x+1,y+4,x+3,y+4);}
void traco3(int x,int y){line(x+1,y+8,x+3,y+8);}
void traco4(int x,int y){line(x+0,y+1,x+0,y+3);}
void traco5(int x,int y){line(x+4,y+1,x+4,y+3);}
void traco6(int x,int y){line(x+0,y+5,x+0,y+7);}
void traco7(int x,int y){line(x+4,y+5,x+4,y+7);}

void algarismo(int n,int x,int y){
	if(n==0){traco1(x,y);traco3(x,y);traco4(x,y);traco5(x,y);traco6(x,y);traco7(x,y);}
	if(n==1){traco5(x,y);traco7(x,y);}
	if(n==2){traco1(x,y);traco2(x,y);traco3(x,y);traco5(x,y);traco6(x,y);}
	if(n==3){traco1(x,y);traco2(x,y);traco3(x,y);traco5(x,y);traco7(x,y);}
	if(n==4){traco2(x,y);traco4(x,y);traco5(x,y);traco7(x,y);}
	if(n==5){traco1(x,y);traco2(x,y);traco3(x,y);traco4(x,y);traco7(x,y);}
	if(n==6){traco1(x,y);traco2(x,y);traco3(x,y);traco4(x,y);traco6(x,y);traco7(x,y);}
	if(n==7){traco1(x,y);traco5(x,y);traco7(x,y);}
	if(n==8){traco1(x,y);traco2(x,y);traco3(x,y);traco4(x,y);traco5(x,y);traco6(x,y);traco7(x,y);}
	if(n==9){traco1(x,y);traco2(x,y);traco3(x,y);traco4(x,y);traco5(x,y);traco7(x,y);}
}

void blitInt(int n,int x,int y){
	int base,padding=0;
	if(n<0){
		line(x+1,y+4,x+2,y+4);
		n=n*-1;
	}
	for(base=10;n>=base;base=base*10);
	while(base>9){
		algarismo((n%base)/(base/10),x+(padding++*7),y);
		base=base/10;
	}
}
