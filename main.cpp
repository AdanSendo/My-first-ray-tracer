#include"tools.h"
#include<cstdlib>
const int w=640;
const int h=480;

cam camara(vec(0,0,4),vec(0,0,0));

int main()
{
	screen sc(640,480);
	vec pl(0,10,6);
	sphere bola(vec(-2,2,-10),6,vec(1,1,1));
	
	for(int i=0;i<h;++i)
	{
		for(int j=0;j<w;++j)
		{
			float dx =2*( ( j+0.5f) / (float)w )-1;  
			float dy =1-2*( (i+0.5f) / (float)h );
			ray r;
			camara.launch(r,dx,dy);
			rec sha;
			sha.e=camara.e;
			float t;
			if(bola.hit(r,t,sha))
			{
				sc.pixel(i,j)=sha.shading(pl);	
			}else
			{
				sc.pixel(i,j)=vec(0,0,0);
			}
		}
	}
	sc.render();
}
