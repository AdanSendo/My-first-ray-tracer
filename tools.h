#pragma once
#include<cmath>
#include<fstream>
#include<vector>
#include<limits>
#include<iostream>


using std::ofstream;
using std::vector;
using std::cout;

const float TMIN=0.1f;
//Vector
	 
	struct vec
	{
		float x,y,z;
		vec(){x=y=z=0;}
		vec(float a,float b,float c):x(a),y(b),z(c){}
		vec& operator = (vec a) {x=a.x , y=a.y , z=a.z; }
		vec operator - () { return vec{-x,-y,-z}; }
		vec operator + (vec a) { return  {x+a.x, y+a.y, z+a.z}; }
		vec operator - (vec a) { return  {x-a.x, y-a.y, z-a.z}; }
		vec operator * (vec a) { return  {x*a.x, y*a.y, z*a.z}; }
		vec operator * (float s) { return {x*s, y*s, z*s}; }
	
	};

	float dot(vec a, vec b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
	vec norm(vec a) { float s=1/sqrt(dot(a,a)); return {a.x*s, a.y*s, a.z*s}; }
	vec cross(vec a, vec b) 
	{ 
		float cx=a.y*b.z - a.z*b.y;
		float cy=a.z*b.x - a.x*b.z;
		float cz=a.x*b.y - a.y*b.x;
		return {cx,cy,cz};
	}
	vec reflect(vec a, vec n)
	{
		return a-(n*(2*dot(a,n)));
	}

//Ray
	
	struct ray
	{
		vec o;
		vec d;
		vec p(float t) { return o+(d*t); }
	
	};

//Rec
	struct rec
	{
		vec e;
		vec p;
		vec n;
		vec c;
		vec shading(vec& luz)
		{
			vec toluz=norm(luz-p);
			vec toeye=norm(e-p);
			vec ref=reflect(-toeye,n);
			vec ambient=c*0.1;
			vec diffuse= c*std::max(dot(toluz,n),0.0f);
			float spec=powf(std::max(dot(ref,n),0.0f),16);
			vec specular=c*spec;
			return (ambient+diffuse+specular)*c*0.4;
		}
	};
//Sphere
	struct sphere
	{
		vec c;
		float r;
		vec color;
		sphere(vec cen,float ra,vec co):c(cen),r(ra),color(co){}
		~sphere(){ cout<<"sphere eliminated!\n";}
		bool hit(ray& ra,float &t,rec& srec)
		{
			vec oc = ra.o-c;
			float a = dot(ra.d,ra.d);
			float b = dot(oc*2,ra.d);
			float cc = (dot(oc,oc))-r*r;
			float d = b*b - 4*a*cc;
			if(d<0) return false;
			
			float e=sqrt(d);

			t=(-b-e) / 2*a;
			if(t<TMIN) t=(-b+e) / 2*a;
			if(t<TMIN) return false;


			srec.p = ra.p(t);
			srec.n = norm(srec.p-c);
			srec.c = color;
			return true;
		}
	};

//Camera

	struct cam
	{
		vec e;
		vec w;
		vec u;
		vec v;
		cam(vec eye,vec look)
		{
			e=eye;
			w=-(look-e);
			u=cross(vec(0,1,0),w);
			v=cross(w,u);

			w=norm(w);
			u=norm(u);
			v=norm(v);
				
		}
		void launch(ray& r, float i,float j)
		{
			r.o = e;
			vec a = u*j;
			vec b = v*i;
			vec c = -w;
			vec d = norm(a+b+c);
			r.d = d;
		}
	};
//Screen

	class screen
	{
			int W,H;
			vector<vec>pixeles;
		public:
			screen(int w,int h):W(w),H(h){ pixeles=vector<vec>(W*H); }
			vec& pixel(int i,int j) { return pixeles[i*W + j]; }
			void render()
			{
				ofstream salida("scene.ppm");
				salida<<"P3\n"<<W<<" "<<H<<"\n255\n";
				for(vec& v:pixeles)
				{
					int r = 255.99*v.x;
					int g = 255.99*v.y;
					int b = 255.99*v.z;
					salida<<r<<" "<<g<<" "<<b<<"\n";
				}
			}
	};
		
