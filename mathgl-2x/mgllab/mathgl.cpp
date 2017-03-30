/* mathgl.cpp is part of UDAV
 * Copyright (C) 2007-2014 Alexey Balakin <mathgl.abalakin@gmail.ru>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "mgl2/mgl.h"
#include "mgllab.h"
//-----------------------------------------------------------------------------
mglParse *Parse=0;
//-----------------------------------------------------------------------------
void udav_error(const char *Message, void *v)
{	((Fl_MGL*)v)->status->label(Message);	}
mreal udav_delay(void *v)
{	return ((Fl_MGL*)v)->delay;	}
void udav_reload(void *v)
{	Parse->RestoreOnce();	((Fl_MGL*)v)->update();	}
//-----------------------------------------------------------------------------
void udav_next(void *v)	{	((Fl_MGL*)v)->next_frame();	}
void Fl_MGL::next_frame()
{
	size_t n=anim.size();
	if(n==0 && animate_cb(this))	{	gr->toggle_sshow();	return;	}
	cur = (cur+1)%n;
	Parse->AddParam(0,anim[cur].c_str());
	update();
}
//-----------------------------------------------------------------------------
void udav_prev(void *v)	{	((Fl_MGL*)v)->prev_frame();	}
void Fl_MGL::prev_frame()
{
	size_t n=anim.size();
	if(n==0 && animate_cb(this))	{	gr->toggle_sshow();	return;	}
	cur = (cur+n-1)%n;
	Parse->AddParam(0,anim[cur].c_str());
	update();
}
//-----------------------------------------------------------------------------
Fl_MGL::Fl_MGL(Fl_MGLView *GR)
{
	if(!Parse)	Parse = new mglParse;
	Parse->AllowSetSize(true);
	gr = GR;	gr->par = this;
	gr->next = udav_next;	gr->delay = udav_delay;
	gr->prev = udav_prev;	gr->reload = udav_reload;
	gr->FMGL->set_draw(this);
	gr->FMGL->set_show_warn(false);
	a1=a2=0;	da=1;	cur=0;	delay=0.5;
}
//-----------------------------------------------------------------------------
Fl_MGL::~Fl_MGL()	{}
//-----------------------------------------------------------------------------
int Fl_MGL::Draw(mglGraph *gr)
{
	Parse->Execute(gr,script.c_str());
	if(textbuf)
	{
		char *text = textbuf->text();
		Parse->Execute(gr,text);
		free(text);
	}
	status->label(gr->Message());
	return 0;
}
//-----------------------------------------------------------------------------
void Fl_MGL::update()
{
	// NOTE: hint for old style View(). May be I should remove it!
	char *text = textbuf->text();
	if(script.empty() || !text || !strstr(text,"rotate"))
		mgl_rotate(gr->get_graph(),0,0,0);

	gr->update();
	for(long i=0;i<Parse->GetNumVar();i++)
	{
		mglDataA *v = Parse->GetVar(i);
		if(v && v->o)	((TableWindow *)v->o)->update(v);
	}
}
//-----------------------------------------------------------------------------
void add_suffix(char *fname, const char *ext)
{
	long n=strlen(fname);
	if(n>4 && fname[n-4]=='.')
	{	fname[n-3]=ext[0];	fname[n-2]=ext[1];	fname[n-1]=ext[2];	}
	else	{	strcat(fname,".");	strcat(fname,ext);	}

}
//-----------------------------------------------------------------------------
class ArgsDlg : public GeneralDlg
{
	Fl_Input *arg[10];
public:
	void cb_ok()
	{
		for(int i=0;i<10;i++)	Parse->AddParam(i,arg[i]->value());
		hide();
	}
	void set(int id, const char *val)
	{
		if(id>=0 && id<10)
		{	arg[id]->value(val);	Parse->AddParam(id,val);	}
	}
	void init()
	{	// NOTE I'm not sure that I need to get current arguments. So keep function empty.
	}
	ArgsDlg()
	{
		w = new Fl_Double_Window(290, 320, mgl_gettext("Set script arguments"));
		arg[1] = new Fl_Input(5, 20, 135, 30, mgl_gettext("String for $1"));
		arg[1]->align(FL_ALIGN_TOP_LEFT);
		arg[2] = new Fl_Input(150, 20, 135, 30, mgl_gettext("String for $2"));
		arg[2]->align(FL_ALIGN_TOP_LEFT);
		arg[3] = new Fl_Input(5, 75, 135, 30, mgl_gettext("String for $3"));
		arg[3]->align(FL_ALIGN_TOP_LEFT);
		arg[4] = new Fl_Input(150, 75, 135, 30, mgl_gettext("String for $4"));
		arg[4]->align(FL_ALIGN_TOP_LEFT);
		arg[5] = new Fl_Input(5, 130, 135, 30, mgl_gettext("String for $5"));
		arg[5]->align(FL_ALIGN_TOP_LEFT);
		arg[6] = new Fl_Input(150, 130, 135, 30, mgl_gettext("String for $6"));
		arg[6]->align(FL_ALIGN_TOP_LEFT);
		arg[7] = new Fl_Input(5, 185, 135, 30, mgl_gettext("String for $7"));
		arg[7]->align(FL_ALIGN_TOP_LEFT);
		arg[8] = new Fl_Input(150, 185, 135, 30, mgl_gettext("String for $8"));
		arg[8]->align(FL_ALIGN_TOP_LEFT);
		arg[9] = new Fl_Input(5, 240, 135, 30, mgl_gettext("String for $9"));
		arg[9]->align(FL_ALIGN_TOP_LEFT);
		arg[0] = new Fl_Input(150, 240, 135, 30, mgl_gettext("String for $0"));
		arg[0]->align(FL_ALIGN_TOP_LEFT);
		Fl_Button* o = new Fl_Button(60, 290, 75, 25, mgl_gettext("Cancel"));
		o->callback(cb_dlg_cancel, this);
		o = new Fl_Return_Button(155, 290, 75, 25, mgl_gettext("Set"));
		o->callback(cb_dlg_ok, this);
		w->set_modal();	w->end();
	}
} args_dlg;
//-----------------------------------------------------------------------------
void args_dlg_cb(Fl_Widget *, void *)	{	args_dlg.show();	}
void argument_set(int id, const char *val)	{	args_dlg.set(id,val);	}
//-----------------------------------------------------------------------------
void cb_anim_put(Fl_Widget *, void *);
void cb_dlg_only(Fl_Widget*,void *v)	{	((Fl_Round_Button*)v)->setonly();	}
class AnimateDlg : public GeneralDlg
{
public:
	Fl_MGL* dr;
	AnimateDlg()
	{
		w = new Fl_Double_Window(335, 350, mgl_gettext("Animation"));
		new Fl_Box(10, 5, 315, 25, mgl_gettext("Redraw picture for $0 equal to:"));
		rt = new Fl_Round_Button(10, 30, 200, 25, mgl_gettext("strings in lines below"));
		rt->callback(cb_dlg_only, rt);
		rv = new Fl_Round_Button(220, 30, 105, 25, mgl_gettext("values"));
		rv->callback(cb_dlg_only, rv);
		txt = new Fl_Multiline_Input(10, 60, 200, 250);
		x0 = new Fl_Float_Input(220, 80, 105, 25, mgl_gettext("from"));			x0->align(FL_ALIGN_TOP_LEFT);
		x1 = new Fl_Float_Input(220, 130, 105, 25, mgl_gettext("to"));			x1->align(FL_ALIGN_TOP_LEFT);
		dx = new Fl_Float_Input(220, 180, 105, 25, mgl_gettext("with step"));	dx->align(FL_ALIGN_TOP_LEFT);
		
		Fl_Button *o;
		o = new Fl_Button(230, 215, 80, 25, mgl_gettext("Cancel"));	o->callback(cb_dlg_cancel, this);
		o = new Fl_Return_Button(230, 250, 80, 25, mgl_gettext("OK"));o->callback(cb_dlg_ok, this);
		save = new Fl_Check_Button(220, 285, 105, 25, mgl_gettext("save slides"));
		save->tooltip(mgl_gettext("Keep slides in memory (faster animation but require more memory)"));
		save->down_box(FL_DOWN_BOX);	save->deactivate();
		
		o = new Fl_Button(10, 315, 100, 25, mgl_gettext("Put to script"));	o->callback(cb_anim_put,w);
		dt = new Fl_Float_Input(220, 315, 105, 25, mgl_gettext("Delay (in sec)"));//	dx->align(FL_ALIGN_TOP_LEFT);
		w->end();
	}
	void init()
	{
		if(e)	dr = e->draw;
		if(dr && dr->da*(dr->a2-dr->a1)>0)
		{
			char buf[128];	rv->setonly();
			snprintf(buf,127,"%g",dr->a1);	x0->value(buf);
			snprintf(buf,127,"%g",dr->a2);	x1->value(buf);
			snprintf(buf,127,"%g",dr->da);	dx->value(buf);
		}
		else if(dr)
		{
			rt->setonly();
			std::string str;
			for(size_t i=0;i<dr->anim.size();i++)
				str += dr->anim[i]+'\n';
			txt->value(str.c_str());
		}
	}
	void prepare()
	{
		result.clear();
		if(dr)	dr->anim.clear();
		if(rv->value())
		{
			const char *s1=x0->value(), *s2=x1->value(), *s3=dx->value();
			double a1=s1?atof(s1):NAN, a2=s2?atof(s2):NAN, a3=s3?atof(s3):1;
			if(a3*(a2-a1)>0)
			{
				result = result + "##c "+s1+' '+s2+' '+(s3?s3:"1")+'\n';
				if(dr)
				{
					dr->a1=a1;	dr->a2=a2;	dr->da=a3;
					for(double a=a1;a3*(a2-a)>=0;a+=a3)
					{
						char buf[128];	snprintf(buf,128,"%g",a);
						dr->anim.push_back(buf);
					}
				}
			}
			else	fl_alert(mgl_gettext("Incompatible loop parameters!"));
		}
		else if(rt->value())
		{
			const char *s = txt->value();
			while(s && *s)
			{
				const char *j = strchr(s,'\n');
				size_t len = j?(j-s):strlen(s);
				std::string val(s,len);
				if(dr)	dr->anim.push_back(val);
				result = result+"##a "+val+'\n';
				s=j?j+1:NULL;
			}
		}
		else	fl_message(mgl_gettext("No selection. So nothing to do"));
		
	}
	void into_script()
	{
		prepare();
		if(e)
		{
			int p = textbuf->line_start(e->editor->insert_position());
			textbuf->insert(p, (result+'\n').c_str());
		}
	}
	void cb_ok()
	{
		if(!dr)	return;
		prepare();
		const char *s = dt->value();
		if(s && *s)	dr->delay = atof(s);
		hide();
	}
protected:
	bool swap;
	Fl_Round_Button *rt, *rv;
	Fl_Multiline_Input *txt;
	Fl_Float_Input *x0, *x1, *dx, *dt;
	Fl_Check_Button *save;
	void create_dlg();
} animate_dlg;
//-----------------------------------------------------------------------------
void animate_dlg_cb(Fl_Widget *, void *v)
{	animate_dlg.e = (ScriptWindow*)v;	animate_dlg.show();	}
void cb_anim_put(Fl_Widget *, void *)	{	animate_dlg.into_script();	}
//-----------------------------------------------------------------------------
bool animate_cb(Fl_MGL *d)
{	animate_dlg.dr = d;	animate_dlg.show();	return animate_dlg.wait();	}
//-----------------------------------------------------------------------------
void fill_animate(const char *text, Fl_MGL *dr)
{
	char tmp[4]="#$0";
	for(int i=0;i<10;i++)	// first read script arguments (if one)
	{
		tmp[2] = '0'+i;
		const char *str=strstr(text,tmp);
		if(str)
		{
			str+=3;
			while(*str>0 && *str<=' ' && *str!='\n')	str++;
			if(*str>' ')
			{
				size_t j=0;	while(str[j]>' ')	j++;
				std::string val(str,j);
				argument_set(i,val.c_str());
			}
		}
	}
	dr->anim.clear();	dr->a1=dr->a2=0;	dr->da=1;	// reset animation
	const char *str = strstr(text, "##c");
	if(str)	// this is animation loop
	{
		double a1=0, a2=0, da=1;
		int res=sscanf(str+3, "%lg%lg%lg", &a1, &a2, &da);
		da = res<3?1:da;
		if(res>2 && da*(a2-a1)>0)
		{
			dr->a1=a1;	dr->a2=a2;	dr->da=da;
			for(double a=a1;da*(a2-a)>=0;a+=da)
			{
				char buf[128];	snprintf(buf,128,"%g",a);
				dr->anim.push_back(buf);
			}
			return;
		}
	}
	str = strstr(text, "##a");
	while(str)
	{
		str += 3;
		while(*str>0 && *str<=' ' && *str!='\n')	str++;
		if(*str>' ')
		{
			size_t j=0;	while(str[j]>' ')	j++;
			std::string val(str,j);
			dr->anim.push_back(val);
		}
		str = strstr(str, "##a");
	}
}
//-----------------------------------------------------------------------------
