                
/*


    Gestion de la souris : zi mouse!


*/
  
class MOUSE { public:
    int act_x; // pos pendant
    int act_y;
    int prev_x; // pos avant
    int prev_y;
    int button[3];
    int action[2];
    int obj_actif; // objet activé par la mouse!
    public :
    int vider();
    int push(int zb,int zx,int zy);
    int release(int zb,int zx,int zy);
    int move(int zx,int zy);
    int archive(int obj);
    int flash_button(int a);
    int get_dragging(int *zx,int *zy);
   // int selection();
    };
int MOUSE::vider()
{
    int i;
    
    for(i=0;i<2;i++) button[i] = action[i] = 0;
    return 1;
}
int MOUSE::flash_button(int a)
{ // simule la pression des boutons!!! cf Editage
    action[1] = a;
    action[0] = 3;
    return 1;
}
int MOUSE::push(int zb,int zx,int zy)
{
    act_x = prev_x = zx;
    act_y = prev_y = zy;
    button[zb] = zb;
    button[0] = button[1] + button[2];
    action[1] = button[0];
    action[0] = 0; // il faut relacher le bouton
    
    // selection simple
    return 1;
}
int MOUSE::release(int zb,int zx,int zy)
{
    act_x = zx;
    act_y = zy;
    button[zb] = 0;
    button[0] = button[1] + button[2];
    if(button[0] == 1) action[0] = 1;
    if(button[0] == 2) action[0] = 3;
    if(button[0] == 3) action[0] = 5;
    // si = 2 ou 4 selection type drag!
    return 1;
}
int MOUSE::move(int zx,int zy)
{
    int a,b;
    
    act_x = zx;
    act_y = zy;
    action[0] = 1; // à priori simple deplacement
    if(button[0] == 1) // Dragging ?
     { a = act_x - prev_x;
       if(a<0) a = prev_x - act_x;
       b = act_y - prev_y;
       if(b<0) b = prev_y - act_y;
       if((a>12)||(b>12)) action[0] = 2; // Dragging reconnu 
     }    
    if(button[0] == 2) // Dragging ?
     { a = act_x - prev_x;
       if(a<0) a = prev_x - act_x;
       b = act_y - prev_y;
       if(b<0) b = prev_y - act_y;
       if((a>12)||(b>12)) action[0] = 4; // Dragging reconnu 
     }    
    return 1;
}
int MOUSE::archive(int obj)
{ // archive(-1) renvoie l'objet en cours !
    if(obj >= 0) obj_actif = obj;
    return obj_actif;
}
int MOUSE::get_dragging(int *zx,int *zy)
{
    *zx = act_x - prev_x;
    *zy = act_y - prev_y;
    return action[0];
}

