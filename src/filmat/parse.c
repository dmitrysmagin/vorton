/*
 * Vorton - Highway Encounter SDL Remake Game.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */



#include "filmat.h"
#include "ML_Lib.h"

static IMLDocument doc=NULL;

#define STR_VORTON	"vortonMap"
#define STR_VERSION	"version"
#define STR_NVER	"1.0"
#define STR_MAP		"map"
#define STR_LEVEL	"level"
#define STR_OBJ		"obj"
#define STR_TYPE	"type"
#define STR_X		"x"
#define STR_Y		"y"
#define STR_ROT		"rot"
#define STR_MOVE	"move"
#define STR_STATE	"state"
#define STR_SIZE_X	"size_x"
#define STR_SIZE_Y	"size_y"
#define STR_VEL		"vel"
#define STR_ITER	"iter"
#define sign_str	"sign"
#define STR_VEC_X	"vec_x"
#define STR_VEC_Y	"vec_y"

#define STR_TYPE_BLOQ	"BLOQ"
#define STR_TYPE_DUST	"DUSTBIN"
#define STR_TYPE_FLAME	"FLAME"
#define STR_TYPE_DEFL1	"DEFL1"
#define STR_TYPE_DEFL2	"DEFL2"
#define STR_TYPE_CICLO	"CYCLOP"
#define STR_TYPE_ROBOT	"ROBOT"
#define STR_TYPE_MOUTH	"MOUTH"
#define STR_TYPE_MINE	"MINE"
#define STR_TYPE_DISC	"DISC"
#define STR_TYPE_BRICK	"BRICK"
#define STR_TYPE_FIRE	"FIRE"
#define STR_TYPE_TILE	"TILE"



static int myatoi(char *str)
{
	int ret=0,sign=1;

	if (str)
	{
		int i, mul;
		for(i=strlen(str)-1,mul=1;i>=0;i--,mul*=10)
		{
			if (str[i]=='-')
				sign=-1;
			else if (str[i]=='+')
				sign=1;
			else if (str[i]>='0' && str[i]<='9')
				ret+=(mul*(str[i]-'0'));
			else return -123456789;
		}
	}
	return (ret*sign);
}


static int openDoc(char *name)
{
	IMLProcessInstruction pi=NULL;
	IMLAttribute pa=NULL;
	int i,ret=0;
	char *tmp_str;

	DocumentFromCOM(IID_IMLDocument,&doc);
	if(! doc->LoadFile(doc,name))
	{
		i=doc->GetSingleProcessInstruction(doc,STR_VORTON,&pi,0);
		if (i>=0)
		{
			if (pi->GetAttribute(pi,STR_VERSION,&pa)>=0)
			{
				if (pa->GetData(pa,&i,(void *)&tmp_str)>=0)
				{
					if (i==strlen(STR_NVER))
					{
						if (strncmp(tmp_str,STR_NVER,i))
							ret=-5;
					} else ret=-5;
				} else ret=-4;
			} else ret=-3;
		} else ret=-2;
	} else ret=-1;
	
	if (pa)
		pa->Release(pa);
	if (pi)
		pi->Release(pi);
	if (ret && doc)
	{
		doc->Release(doc);
		doc=NULL;
	}
	if (tmp_str)
		free(tmp_str);
	return ret;
}

static void closeDoc(void)
{
	if (doc)
	{
		doc->Release(doc);
		doc=NULL;
	}
}

static int getNewObject(void)
{
	int n_obj=search_free_obj();

	if (n_obj>=0)
	{
		filmat_obj[n_obj].push_x=0;
		filmat_obj[n_obj].push_y=0;
		filmat_obj[n_obj].position=n_obj;
		filmat_obj[n_obj].rot=0;
		filmat_obj[n_obj].state=1;
		filmat_obj[n_obj].vel=2;
		filmat_obj[n_obj].move_type=9;
		filmat_obj[n_obj].obj_type=FIL_TYPE_MINE;
		filmat_obj[n_obj].img=FIL_IMG_MINE_0;
		filmat_obj[n_obj].size_x=FIL_TILE_SIZE;
		filmat_obj[n_obj].size_y=FIL_TILE_SIZE;
		filmat_obj[n_obj].iter=1;
		filmat_obj[n_obj].vec_x=0;
		filmat_obj[n_obj].vec_y=0;
		filmat_obj[n_obj].sign=0;
	}
	return n_obj;
}

static void completObject(int n_obj)
{
	filmat_obj[n_obj].back_x=filmat_obj[n_obj].x;
	filmat_obj[n_obj].back_y=filmat_obj[n_obj].y;
	switch(filmat_obj[n_obj].obj_type)
	{
		case FIL_TYPE_FIRE:
			filmat_fire_init(&filmat_obj[n_obj]);
			break;
		case FIL_TYPE_BRICK:
			filmat_brick_init(&filmat_obj[n_obj]);
			break;
		case FIL_TYPE_DUSTBIN:
			filmat_movable_init(&filmat_obj[n_obj]);
			break;
		case FIL_TYPE_BLOQ:
		case FIL_TYPE_TILE:
			filmat_bloq_init(&filmat_obj[n_obj]);
			break;
		case FIL_TYPE_MINE:
		case FIL_TYPE_CYCLOP:
		case FIL_TYPE_MOUTH:
		case FIL_TYPE_DISC:
		case FIL_TYPE_ROBOT:
			filmat_robot_init(&filmat_obj[n_obj]);
			break;			
	}
}

static char* getElement(IMLElement elem, char *search, int *i)
{
	int ret=0;
	IMLAttribute attr=NULL;
	char *tmp_str=NULL;
	int n;
	
	if (elem->GetAttribute(elem,search,&attr)>=0)
	{
		if (attr->GetData(attr,i,(void *)&tmp_str)>=0)
		{
			tmp_str[*i]=0;
			for(n=0;n<*i;n++)
				if (tmp_str[n]>='a' && tmp_str[n]<='z')
					tmp_str[n]-=('a'-'A');
			ret=-1;
		}
		else
			if (tmp_str)
			{
				free(tmp_str);
				tmp_str=NULL;
			}
	}
	if (attr)
		attr->Release(attr);
	return tmp_str;
}

static int parseDoc(void)
{	
	IMLElement elem=NULL,subelem=NULL;
	IMLAttribute attr=NULL;
	int index=0,n_obj,i,max,ret=0;
	char *tmp_str;

	FLI_InitDefaultFont();

	if (doc)
	{
	 if (doc->GetSingleElement(doc,STR_MAP,&elem,0)>=0)
	 {
	  if (elem->GetAttribute(elem,STR_LEVEL,&attr)>=0)
	  {
	   if (attr->GetData(attr,&i,(void *)&tmp_str)>=0)
	   {
	    tmp_str[i]=0;
	    if (myatoi(tmp_str)!=level)
	    {
		    FLI_FreeDefaultFont();
		    return -1;
            }
	    free(tmp_str);
	    tmp_str=NULL;
	    attr->Release(attr);
	    attr=NULL;
	    max=elem->GetNumChilds(elem);
	    while((index=elem->GetSingleElement(elem,STR_OBJ,&subelem,index))>=0)
	    {
		display_gauge_string(index, max, "Parsing");
		attr=NULL;
		if ((n_obj=getNewObject())<0)
		{
		        FLI_FreeDefaultFont();
			return -1;
		}
		if ((tmp_str=getElement(subelem,STR_TYPE,&i))!=NULL)
		{
			if (!strcmp(tmp_str,STR_TYPE_DUST))
				filmat_obj[n_obj].obj_type=FIL_TYPE_DUSTBIN;
			else
			if (!strcmp(tmp_str,STR_TYPE_BLOQ))
				filmat_obj[n_obj].obj_type=FIL_TYPE_BLOQ;
			else
			if (!strcmp(tmp_str,STR_TYPE_FLAME))
				filmat_obj[n_obj].obj_type=FIL_TYPE_FLAME;
			else
			if (!strcmp(tmp_str,STR_TYPE_DEFL1))
			{
				filmat_obj[n_obj].obj_type=FIL_TYPE_DEFL1;
				filmat_obj[n_obj].img=FIL_IMG_DEFL1;
			}
			else
			if (!strcmp(tmp_str,STR_TYPE_DEFL2))
			{
				filmat_obj[n_obj].obj_type=FIL_TYPE_DEFL2;
				filmat_obj[n_obj].img=FIL_IMG_DEFL2;
			}
			else
			if (!strcmp(tmp_str,STR_TYPE_CICLO))
				filmat_obj[n_obj].obj_type=FIL_TYPE_CYCLOP;
			else
			if (!strcmp(tmp_str,STR_TYPE_ROBOT))
				filmat_obj[n_obj].obj_type=FIL_TYPE_ROBOT;
			else
			if (!strcmp(tmp_str,STR_TYPE_MOUTH))
				filmat_obj[n_obj].obj_type=FIL_TYPE_MOUTH;
			else
			if (!strcmp(tmp_str,STR_TYPE_MINE))
			{
				filmat_obj[n_obj].obj_type=FIL_TYPE_MINE;
				filmat_obj[n_obj].img=FIL_IMG_MINE_0;
			}
			else
			if (!strcmp(tmp_str,STR_TYPE_DISC))
				filmat_obj[n_obj].obj_type=FIL_TYPE_DISC;
			else
			if (!strcmp(tmp_str,STR_TYPE_BRICK))
				filmat_obj[n_obj].obj_type=FIL_TYPE_BRICK;
			else
			if (!strcmp(tmp_str,STR_TYPE_FIRE))
				filmat_obj[n_obj].obj_type=FIL_TYPE_FIRE;
			else
			if (!strcmp(tmp_str,STR_TYPE_TILE))
				filmat_obj[n_obj].obj_type=FIL_TYPE_TILE;
			free(tmp_str);
		}
		if ((tmp_str=getElement(subelem,STR_X,&i))!=NULL)
		{
			filmat_obj[n_obj].x=FIL_MIN_X+myatoi(tmp_str);
			free(tmp_str);
		}
		if ((tmp_str=getElement(subelem,STR_Y,&i))!=NULL)
		{
			filmat_obj[n_obj].y=160+myatoi(tmp_str);
			free(tmp_str);
		}
		if ((tmp_str=getElement(subelem,STR_ROT,&i))!=NULL)
		{
			filmat_obj[n_obj].rot=myatoi(tmp_str);
			free(tmp_str);
		}
		if ((tmp_str=getElement(subelem,STR_VEL,&i))!=NULL)
		{
			filmat_obj[n_obj].vel=myatoi(tmp_str);
			free(tmp_str);
		}
		if ((tmp_str=getElement(subelem,STR_STATE,&i))!=NULL)
		{
			filmat_obj[n_obj].state=myatoi(tmp_str);
			free(tmp_str);
		}
		if ((tmp_str=getElement(subelem,STR_MOVE,&i))!=NULL)
		{
			filmat_obj[n_obj].move_type=myatoi(tmp_str);
			free(tmp_str);
		}
		if ((tmp_str=getElement(subelem,STR_SIZE_X,&i))!=NULL)
		{
			filmat_obj[n_obj].size_x=myatoi(tmp_str);
			free(tmp_str);
		}
		if ((tmp_str=getElement(subelem,STR_SIZE_Y,&i))!=NULL)
		{
			filmat_obj[n_obj].size_y=myatoi(tmp_str);
			free(tmp_str);
		}
		if ((tmp_str=getElement(subelem,STR_ITER,&i))!=NULL)
		{
			filmat_obj[n_obj].iter=myatoi(tmp_str);
			free(tmp_str);
		}
		if ((tmp_str=getElement(subelem,STR_VEC_X,&i))!=NULL)
		{
			filmat_obj[n_obj].vec_x=myatoi(tmp_str);
			free(tmp_str);
		}
		if ((tmp_str=getElement(subelem,STR_VEC_Y,&i))!=NULL)
		{
			filmat_obj[n_obj].vec_y=myatoi(tmp_str);
			free(tmp_str);
		}
		if ((tmp_str=getElement(subelem,sign_str,&i))!=NULL)
		{
			filmat_obj[n_obj].sign=myatoi(tmp_str);
			free(tmp_str);
		}
	        tmp_str=NULL;
		index++;
		if (subelem)
			subelem->Release(subelem);
		subelem=NULL;
		completObject(n_obj);
		n_obj=-1;
	    }
	    if (subelem)
		subelem->Release(subelem);
	    subelem=NULL;
           } else ret=-4;
	  } else ret=-3;
	 } else ret=-2;
	} else ret=-1;
	
	if (attr)
		attr->Release(attr);
	if (subelem)
		subelem->Release(subelem);
	if (elem)
		elem->Release(elem);
	if (tmp_str)
		free(tmp_str);
	display_gauge_string(100, 100, "Parsing Ok.");
        FLI_FreeDefaultFont();
	return ret;
}




int filmat_parse_map(void)
{
	char *map_name=(char *)calloc(1024,1);
	int ret=0;
	sprintf(map_name,"Pre-parsing: %s%i%s%s",DATA_PREFIX,level,FILE_SEPARATOR,"level.map");
	display_gauge_only(map_name);
	sprintf(map_name,"%s%i%s%s",DATA_PREFIX,level,FILE_SEPARATOR,"level.map");
	if (!openDoc(map_name))
	{
		if (parseDoc())
			ret=-1;
	}
	else
		ret=-2;

	closeDoc();
	free(map_name);
	return ret;
}

