#include <jni.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string.h>
#include "shader.h"

using namespace std;
struct face
{
    int vert_[50];
    int vert_n[50];
    int vert_t[50];
    int vert_size;
    int last_vert_pos;

    face()
    {
        memset(vert_, -1, sizeof(vert_));
        memset(vert_n, -1, sizeof(vert_n));
        memset(vert_t, -1, sizeof(vert_t));
    }
};

struct vert
{
    float val[4];
};

struct obj_info
{
    int id;
    int vert_size;
    float x_min;
    float y_min;
    float z_min;
    float x_max;
    float y_max;
    float z_max;
    std::string name;
};

struct obj_info dimension;
//map<struct dim*, vector<face*>> objects;
vector<face*> faces;
vector<vert*> vertices;
map<std::pair<int , struct obj_info*>, vector<face*>> object_list;

vector<vert*> vertices_obj;
vector<vert*> vertices_nrm_obj;
vector<vert*> vertices_txt_obj;
int max_faces=0;
int init_dim_x, init_dim_y, init_dim_z;

size_t found;
int object_index=-1;
int line_num = 0;
int vert_size=0;
struct obj_info* t_dim;
std::string obj_name;

static vector<float> vertices_group;
static vector<unsigned short> indices;

int get_dimension(float val, int axis)
{
    switch(axis)
    {
        case 0:
            if(init_dim_x)
            {
                if(val < dimension.x_min)
                {
                    dimension.x_min = val;
                }
                if(val > dimension.x_max)
                {
                    // printf("%f, %f\r\n", dimension.x_max, val);
                    dimension.x_max = val;
                }
            }
            else
            {
                init_dim_x =1;
                dimension.x_max =val;
                dimension.x_min =val;
                // printf("%f, %d\r\n", val, axis);
            }
        break;
        case 1:
            if(init_dim_y)
            {
                if(val < dimension.y_min)
                {
                    dimension.y_min = val;
                }
                if(val > dimension.y_max)
                {
                    dimension.y_max = val;
                }
            }
            else
            {
                init_dim_y=1;
                dimension.y_max = dimension.y_min = val;
                // printf("%f, %d\r\n", val, axis);
            }
        break;
        case 2:
            if(init_dim_z)
            {
                if(val < dimension.z_min)
                {
                    dimension.z_min = val;
                }
                if(val > dimension.z_max)
                {
                    dimension.z_max = val;
                }
            }
            else
            {
                init_dim_z=1;
                dimension.z_max = dimension.z_min = val;
                // printf("%f, %d\r\n", val, axis);
            }
        break;
        default:;
    }
    return 0;
}

//int parse_vertices(string str_file, string delimeter)
int parse_vertices(std::string line, string delimiter)
{
    //stringstream ctx(str_file);
    // std::cout << "Parsing operation started for " << delimeter << std::endl;
#if 0    
    size_t found = ctx.find("f");
    while(found != string::npos)
    {
        cout << found << endl;
        found = ctx.find("f", found+1);
        for(std::string::iterator it = ctx.)
        {

        }
        face *n_face = new face;
        n_face->a = 
    }
#endif
    bool vertices_info_flag = false;
    stringstream temp;
    vert *n_vert;
    int vert_index=0;
    //for (string line; getline(ctx, line); )

    n_vert = new vert;
    vert_index=0;
    
    string::iterator it = line.begin();

    while (it != line.end())
    {
        if(*it == ' ')
        {
            if(temp.str().length()>0)
            {
                if(!delimiter.compare("v"))
                {
                    get_dimension( std::stof(temp.str()), vert_index);
                }
                n_vert->val[vert_index] = std::stof(temp.str());
                // cout << "face index: " << vert_index << "val: " << n_vert->val[vert_index] << endl;
                temp.str("");
                temp.clear();
                vert_index++;
            }
        }
        if((*it >= '0' && *it <= '9') || *it == '.' || *it == '-')
            temp << *it;
        it++;
    }

    if(temp.str().length()>0)
    {
        if(!delimiter.compare("v"))
        {
            get_dimension( stof(temp.str()), vert_index);
        }
        n_vert->val[vert_index] = stof(temp.str());
        // cout << "face index: " << faces_index << "val: " << n_face->val[faces_index] << endl;
        temp.str("");
        temp.clear();
        vert_index++;
    }

    if(n_vert != nullptr)
    {
        // vertices.push_back(n_vert);
        if(!delimiter.compare("v"))
        {
            vertices_obj.push_back(n_vert);
        }
        else if(!delimiter.compare("vn"))
        {
            vertices_nrm_obj.push_back(n_vert);
        }
        else if(!delimiter.compare("vt"))
        {
            vertices_txt_obj.push_back(n_vert);
        }
    }

    // vertices.clear();

    return 0;
}

unsigned short* get_indices()
{
    return indices.data();
}

float* get_vertices()
{
    return vertices_group.data();
}

void update_indices() {
    int n=0, tmp=0;
    int offset=0;
    indices.clear();
    for (auto pair: object_list){
        for (auto face: pair.second) {
            n = 1;
            for (int index = 2; index < face->vert_size; index++) {
                //out << face->vert_[tmp]-1-offset <<", " << face->vert_[tmp+n]-1-offset << ", " << face->vert_[tmp+n+1]-1-offset << ", ";
                //out << tmp <<", " << tmp+n << ", " << tmp+n+1 << ", ";
                indices.push_back(tmp);
                indices.push_back(tmp + (index - 1));
                indices.push_back(tmp + index);
                n++;
            }
            tmp += face->vert_size;
        }
    }
}

void update_vertices()
{
    vert* ptr_vertices = nullptr;
    vertices_group.clear();
    for(auto pair : object_list) {
        for (auto face: pair.second) {
            /*Get each vertex per face*/
            for (int index = 0; index < face->vert_size; index++) {
                if (vertices_obj.size()) {
                    ptr_vertices = vertices_obj.at(face->vert_[index] - 1);
                    vertices_group.push_back(ptr_vertices->val[0]);
                    vertices_group.push_back(ptr_vertices->val[1]);
                    vertices_group.push_back(ptr_vertices->val[2]);
                    vertices_group.push_back(1.0);
                }

                if (vertices_txt_obj.size()) {
                    if (face->vert_t[0] == -1) {
                        vertices_group.push_back(0);
                        vertices_group.push_back(0);
                        vertices_group.push_back(0);
                    } else {
                        ptr_vertices = vertices_txt_obj.at(face->vert_t[index] - 1);
                        vertices_group.push_back(ptr_vertices->val[0]);
                        vertices_group.push_back(ptr_vertices->val[1]);
                        vertices_group.push_back(0);
                    }
                }

                if (vertices_nrm_obj.size()) {
                    if (face->vert_n[0] == -1) {
                        vertices_group.push_back(0);
                        vertices_group.push_back(0);
                        vertices_group.push_back(0);
                    } else {
                        ptr_vertices = vertices_nrm_obj.at(face->vert_n[index] - 1);
                        vertices_group.push_back(ptr_vertices->val[0]);
                        vertices_group.push_back(ptr_vertices->val[1]);
                        vertices_group.push_back(ptr_vertices->val[2]);
                    }
                }

                {
                    /*Add diffuse color*/
                    vertices_group.push_back(0);
                    vertices_group.push_back(1);
                    vertices_group.push_back(0);
                }
            }
        }
    }
}

void print_s()
{
    ofstream out;
    vert* vertices;
    int n=0, tmp=0;
    int offset=0;

    out.open("obj_info.c");

    if(!out.is_open())
    {
        return;
    }
    out << "#include <stddef.h>" << endl;
    for(auto pair : object_list)
    {
        tmp=0;
        out << "/*** Obj - "<< pair.first.second->name << "[ " <<pair.first.first << " ]" <<" ***/" << endl;
        out << "/* x [" << pair.first.second->x_min << " / " << pair.first.second->x_max << "], y [" << pair.first.second->y_min << " / " << pair.first.second->y_max <<  "] z [" << pair.first.second->z_min << " / " << pair.first.second->z_max << "] */"<< endl;
        out << "/* w [" << pair.first.second->x_max - pair.first.second->x_min << "], H [" << pair.first.second->y_max - pair.first.second->y_min << "] D [" << pair.first.second->z_max - pair.first.second->z_min << "] */"<< endl;
        out << "/* cx [" << ((pair.first.second->x_max - pair.first.second->x_min)/2)+pair.first.second->x_min << "], cy [" << ((pair.first.second->y_max - pair.first.second->y_min)/2)+pair.first.second->y_min <<  "], cz [" << ((pair.first.second->z_max - pair.first.second->z_min)/2)+pair.first.second->z_min << "] */"<< endl;
        out << "static float vertices[]={" << endl;
        for(auto face : pair.second)
        {
            /*Get each vertex per face*/
            for(int index=0; index < face->vert_size; index++)
            {
                if(vertices_obj.size())
                {
                    vertices = vertices_obj.at(face->vert_[index]-1);
                    out << vertices->val[0] << "," << vertices->val[1] << "," << vertices->val[2] << "," << "1,"<< endl;   
                }

                if(vertices_txt_obj.size())
                {
                    if(face->vert_t[0]==-1)
                    {
                        out << "0,0,0,"<< endl;   
                    }
                    else
                    {
                        vertices = vertices_txt_obj.at(face->vert_t[index]-1);
                        out << vertices->val[0] << "," << vertices->val[1] << "," << "0," << endl;
                    }  
                }

                if(vertices_nrm_obj.size())
                {
                    if(face->vert_n[0]==-1)
                    {
                        out << "0,0,0,"<< endl;   
                    }
                    else
                    {
                        vertices = vertices_nrm_obj.at(face->vert_n[index]-1);
                        out << vertices->val[0] << "," << vertices->val[1] << "," << vertices->val[2] << "," << endl;      
                    }
                }

                {
                    /*Add diffuse color*/
                    out << "1,0,0,"<< endl;    
                }
            }
        }
        out << "};" << endl;
#if 0 
        if(vertices_obj.size())
        {
            out << "static float vertices[]={" << endl;
            for(auto face : pair.second)
            {
                /*Get each vertex per face*/
                for(int index=0; index < face->vert_size; index++)
                {
                    vertices = vertices_obj.at(face->vert_[index]-1);
                    out << vertices->val[0] << "," << vertices->val[1] << "," << vertices->val[2] << "," << "1,"<< endl;   
                }
            }
            out << "};" << endl;
        }

        if(vertices_nrm_obj.size())
        {
            out << "static float normals[]={" << endl;
            for(auto face : pair.second)
            {
                /*Get each normals per face*/
                for(int index=0; index < face->vert_size; index++)
                {
                    vertices = vertices_nrm_obj.at(face->vert_n[index]-1);
                    out << vertices->val[0] << "," << vertices->val[1] << "," << vertices->val[2] << "," << endl;   
                }
            }
            out << "};" << endl;
        }
  
        if(vertices_txt_obj.size())
        {
            out << "static float textures[]={" << endl;
            for(auto face : pair.second)
            {
                /*Get each textures per face*/
                for(int index=0; index < face->vert_size; index++)
                {
                    vertices = vertices_txt_obj.at(face->vert_t[index]-1);
                    out << vertices->val[0] << "," << vertices->val[1] << "," << endl;   
                }
            }
            out << "};" << endl;
        }
#endif
        out << "static float indices[]={" << endl;
        int triangles=0;
        for(auto face : pair.second)
        {
            n=1; 
            for(int index = 2; index <  face->vert_size; index++)
            {
                ++triangles;
                //out << face->vert_[tmp]-1-offset <<", " << face->vert_[tmp+n]-1-offset << ", " << face->vert_[tmp+n+1]-1-offset << ", ";   
                //out << tmp <<", " << tmp+n << ", " << tmp+n+1 << ", ";   
                out << tmp <<", " << tmp+(index-1) << ", " << tmp+index << ", ";   
                n++;
            }
            tmp += face->vert_size;
            out << endl;
        }
        out << "};" << endl;

        out << "/*** End of Obj - "<< pair.first.second->name << "[ " <<pair.first.first << " ] " << \
                "[ TRIANGLES : "<< triangles << " ]" <<" ***/" << endl;
        offset = pair.first.second->vert_size;
        // printf("offset: %d\r\n", offset);
    }
    // out << "int get_vert_size()\n{\nreturn sizeof(vert)/sizeof(vert[0]);\n}" << endl;
    // out << "float * get_vert()\n{\nreturn vert;\n}" << endl;
    out.close();
}

void print()
{
    ofstream out;

    out.open("obj_info.c");

    if(!out.is_open())
    {
        return;
    }
    out << "#include <stddef.h>" << endl;
    out << "static float vert[]={" << endl;
#if 1    
    vert* vertices;
    int n=0;
    for(auto pair: object_list)
    {
        out << "{\r\n" << endl;
        for (auto face: pair.second)
        {
            n=1; 
            for(int index = 0; index <  face->vert_size/2; index++)
            {
                if(vertices_obj.size())
                {
                    vertices = vertices_obj.at(face->vert_[0]-1);
                    out << vertices->val[0] << "," << vertices->val[1] << "," << vertices->val[2] << "," << "1,"<< endl;   
                }
                if(vertices_txt_obj.size())
                {
                    if(face->vert_t[0]!=-1)
                    {

                        vertices = vertices_txt_obj.at(face->vert_t[0]-1);
                        out << vertices->val[0] << "," << vertices->val[1] << "," << "0,"<< endl;   
                    }
                    else
                    {
                        out << "0,0,0,"<< endl;   
                    }
                }
                if(vertices_nrm_obj.size())
                {
                    if(face->vert_n[0]!=-1)
                    {
                        vertices = vertices_nrm_obj.at(face->vert_n[0]-1);
                        out << vertices->val[0] << "," << vertices->val[1] << "," << vertices->val[2] << "," << endl;
                    }
                    else
                    {
                        out << "0,0,0,"<< endl;   
                    }
                }
                // printf("%d, %d\r\n", __LINE__, n);
                { // n
                    if(vertices_obj.size())
                    {
                        vertices = vertices_obj.at(face->vert_[n]-1);
                        out << vertices->val[0] << "," << vertices->val[1] << "," << vertices->val[2] << "," << "1,"<< endl;   
                    }
                    if(vertices_txt_obj.size())
                    {
                        if(face->vert_t[n]!=-1)
                        {
                            vertices = vertices_txt_obj.at(face->vert_t[n]-1);
                            out << vertices->val[0] << "," << vertices->val[1] << "," << "0,"<< endl;   
                        }
                        else
                        {
                            out << "0,0,0,"<< endl;   
                        }
                    }
                    if(vertices_nrm_obj.size())
                    {
                        if(face->vert_n[n]!=-1)
                        {
                            vertices = vertices_nrm_obj.at(face->vert_n[n]-1);
                            out << vertices->val[0] << "," << vertices->val[1] << "," << vertices->val[2] << "," << endl;
                        }
                        else
                        {
                            out << "0,0,0,"<< endl;   
                        }
                    }

                    if(vertices_obj.size())
                    {
                        vertices = vertices_obj.at(face->vert_[n+1]-1);
                        out << vertices->val[0] << "," << vertices->val[1] << "," << vertices->val[2] << "," << "1,"<< endl;   
                    }
                    if(vertices_txt_obj.size())
                    {
                        if(face->vert_t[n+1]!=-1)
                        {
                            vertices = vertices_txt_obj.at(face->vert_t[n+1]-1);
                            out << vertices->val[0] << "," << vertices->val[1] << "," << "0,"<< endl;   
                        }
                        else
                        {
                            out << "0,0,0,"<< endl;   
                        }
                    }
                    if(vertices_nrm_obj.size())
                    {
                        if(face->vert_n[n+1]!=-1)
                        {
                            vertices = vertices_nrm_obj.at(face->vert_n[n+1]-1);
                            out << vertices->val[0] << "," << vertices->val[1] << "," << vertices->val[2] << "," << endl;
                        }
                        else
                        {
                            out << "0,0,0,"<< endl;   
                        }
                    }         
                        
        
                }
                n++;
                // printf("%d, %d\r\n", __LINE__, n);
            }
        }
        out << "}," << endl;
    }
#else   
    for (auto face: faces)
    {
        vert* vertices;

        std::cout << __LINE__ << std::endl;

        if(vertices_obj.size())
        {
            std::cout << face->vert_[0]-1 << " " << __LINE__ << std::endl;
            vertices = vertices_obj.at(face->vert_[0]-1);
            out << vertices->val[0] << "," << vertices->val[1] << "," << vertices->val[2] << "," << "1,"<< endl;   
        }
        if(vertices_txt_obj.size())
        {
            vertices = vertices_txt_obj.at(face->vert_t[0]-1);
            out << vertices->val[0] << "," << vertices->val[1] << "," << "0,"<< endl;   
        }
        if(vertices_nrm_obj.size())
        {
            vertices = vertices_nrm_obj.at(face->vert_n[0]-1);
            out << vertices->val[0] << "," << vertices->val[1] << "," << vertices->val[2] << "," << endl;
        }

        std::cout << __LINE__ << std::endl;

        if(vertices_obj.size())
        {
            vertices = vertices_obj.at(face->vert_[1]-1);
            out << vertices->val[0] << "," << vertices->val[1] << "," << vertices->val[2] << "," << "1,"<< endl;   
        }
        if(vertices_txt_obj.size())
        {
            vertices = vertices_txt_obj.at(face->vert_t[1]-1);
            out << vertices->val[0] << "," << vertices->val[1] << "," << "0,"<< endl;   
        }
        if(vertices_nrm_obj.size())
        {
            vertices = vertices_nrm_obj.at(face->vert_n[1]-1);
            out << vertices->val[0] << "," << vertices->val[1] << "," << vertices->val[2] << "," << endl;
        }
        if(vertices_obj.size())
        {
            vertices = vertices_obj.at(face->vert_[2]-1);
            out << vertices->val[0] << "," << vertices->val[1] << "," << vertices->val[2] << "," << "1,"<< endl;   
        }
        if(vertices_txt_obj.size())
        {
            vertices = vertices_txt_obj.at(face->vert_t[2]-1);
            out << vertices->val[0] << "," << vertices->val[1] << "," << "0,"<< endl;   
        }
        if(vertices_nrm_obj.size())
        {
            vertices = vertices_nrm_obj.at(face->vert_n[2]-1);
            out << vertices->val[0] << "," << vertices->val[1] << "," << vertices->val[2] << "," << endl;
        }

        std::cout << __LINE__ << std::endl;
        
        if(face->vert_[3] != -1)
        {

            if(vertices_obj.size())
            {
                vertices = vertices_obj.at(face->vert_[0]-1);
                out << vertices->val[0] << "," << vertices->val[1] << "," << vertices->val[2] << "," << "1,"<< endl;   
            }
            if(vertices_txt_obj.size())
            {
                vertices = vertices_txt_obj.at(face->vert_t[0]-1);
                out << vertices->val[0] << "," << vertices->val[1] << "," << "0,"<< endl;   
            }
            if(vertices_nrm_obj.size())
            {
                vertices = vertices_nrm_obj.at(face->vert_n[0]-1);
                out << vertices->val[0] << "," << vertices->val[1] << "," << vertices->val[2] << "," << endl;
            }

            if(vertices_obj.size())
            {
                vertices = vertices_obj.at(face->vert_[2]-1);
                out << vertices->val[0] << "," << vertices->val[1] << "," << vertices->val[2] << "," << "1,"<< endl;   
            }
            if(vertices_txt_obj.size())
            {
                vertices = vertices_txt_obj.at(face->vert_t[2]-1);
                out << vertices->val[0] << "," << vertices->val[1] << "," << "0,"<< endl;   
            }
            if(vertices_nrm_obj.size())
            {
                vertices = vertices_nrm_obj.at(face->vert_n[2]-1);
                out << vertices->val[0] << "," << vertices->val[1] << "," << vertices->val[2] << "," << endl;
            }

            if(vertices_obj.size())
            {
                vertices = vertices_obj.at(face->vert_[3]-1);
                out << vertices->val[0] << "," << vertices->val[1] << "," << vertices->val[2] << "," << "1,"<< endl;   
            }
            if(vertices_txt_obj.size())
            {
                vertices = vertices_txt_obj.at(face->vert_t[3]-1);
                out << vertices->val[0] << "," << vertices->val[1] << "," << "0,"<< endl;   
            }
            if(vertices_nrm_obj.size())
            {
                vertices = vertices_nrm_obj.at(face->vert_n[3]-1);
                out << vertices->val[0] << "," << vertices->val[1] << "," << vertices->val[2] << "," << endl;   
            }
        }
        std::cout << __LINE__ << std::endl;
        //cout << "Vert: " << face->vert_[0] << "," << face->vert_[1] << "," << face->vert_[2] << "," << face->vert_[3]<< endl;
        //cout << "vert_n: " << face->vert_n[0] << "," << face->vert_n[1] << "," << face->vert_n[2] << "," << face->vert_n[3]<< endl;
        //cout << "vert_t: " << face->vert_t[0] << "," << face->vert_t[1] << "," << face->vert_t[2] << "," << face->vert_t[3]<< endl;
    }
#endif    
    //cout << endl;
    out << "};" << endl;
    out << "int get_vert_size()\n{\nreturn sizeof(vert)/sizeof(vert[0]);\n}" << endl;
    out << "float * get_vert()\n{\nreturn vert;\n}" << endl;
    out.close();

}

#if 0
void print_faces()
{
    ofstream out;

    out.open("obj_info.c", ios_base::app);

    if(!out.is_open())
    {
        return;
    }

    out << "int indices[]={" << endl;
    for (auto face: faces)
    {
        //cout << "Vert: " << face->vert_[0] << "," << face->vert_[1] << "," << face->vert_[2] << "," << face->vert_[3]<< endl;
        //cout << "vert_n: " << face->vert_n[0] << "," << face->vert_n[1] << "," << face->vert_n[2] << "," << face->vert_n[3]<< endl;
        //cout << "vert_t: " << face->vert_t[0] << "," << face->vert_t[1] << "," << face->vert_t[2] << "," << face->vert_t[3]<< endl;
        out << face->vert_[0]-1 << "," << face->vert_[1]-1 << "," << face->vert_[2]-1 << ",";
        if(face->vert_[3] != -1)
            out << face->vert_[0]-1 << "," << face->vert_[2]-1 << "," << face->vert_[3]-1 << ","; 
        out << endl;
    }

    //cout << endl;
    out << "};" << endl;
    out.close();
}

void print_vertices()
{
    ofstream out;

    out.open("obj_info.c");

    if(!out.is_open())
    {
        return;
    }

    out << "float vertices[]={" << endl; 
    for (auto vertex: vertices_obj)
    {
       // cout << vertex->val[0] << "," << vertex->val[1] << "," << vertex->val[2] << "," << endl;
        out << vertex->val[0] << "," << vertex->val[1] << "," << vertex->val[2] << "," << endl;
    }
    out << "};" << endl; 

    out << "float normals[]={" << endl; 
    for (auto vertex: vertices_nrm_obj)
    {
       // cout << vertex->val[0] << "," << vertex->val[1] << "," << vertex->val[2] << "," << endl;
        out << vertex->val[0] << "," << vertex->val[1] << "," << vertex->val[2] << "," << endl;
    }
    out << "};" << endl; 

    out << "float texture[]={" << endl; 
    for (auto vertex: vertices_txt_obj)
    {
      //  cout << vertex->val[0] << "," << vertex->val[1] << "," << vertex->val[2] << "," << endl;
        out << vertex->val[0] << "," << vertex->val[1] << "," << endl;
    }
    out << "};" << endl; 
    out.close();
}
#endif

int parse_indices(std::string& line)
{
    stringstream temp;
    face *n_face;
    int faces_index=0;
    int face_item=0;
    int last_vert_pos=0;

    string::iterator it = line.begin();
    n_face = new face;
    faces_index=0;
    face_item=1;
    temp.str("");
    temp.clear();

    while (it != line.end())
    {
        if(*it == ' ')
        {
            if(temp.str().length()>0)
            {
                switch(face_item)
                {
                    case 1:
                        n_face->vert_[faces_index] = stoi(temp.str());break;
                    case 2:
                        n_face->vert_t[faces_index] = stoi(temp.str());break;
                    case 3:
                        n_face->vert_n[faces_index] = stoi(temp.str());break;
                    default:;
                }
                face_item++;
                // cout << "face index: " << faces_index << "val: " <<  stoi(temp.str()) << endl;
                temp.str("");
                temp.clear();
                faces_index++;
                face_item=1;
            }
        }
        if(*it == '/')
        {
            if(temp.str().length()>0)
            {
                switch(face_item)
                {
                    case 1:
                        n_face->vert_[faces_index] = stoi(temp.str());
                        if(last_vert_pos < n_face->vert_[faces_index])
                        {
                            last_vert_pos = n_face->vert_[faces_index];
                        }
                        break;
                    case 2:
                        n_face->vert_t[faces_index] = stoi(temp.str());break;
                    case 3:
                        n_face->vert_n[faces_index] = stoi(temp.str());break;
                    default:;
                }
                //cout << "face index: " << faces_index << "val: " << stoi(temp.str()) << endl;
                temp.str("");
                temp.clear();
            }
            face_item++;
        }
        if(*it >= '0' && *it <= '9')
            temp << *it;
        it++;
    }
    if(temp.str().length()>0)
    {
        switch(face_item)
        {
            case 1:
                n_face->vert_[faces_index] = stoi(temp.str());
                if(last_vert_pos < n_face->vert_[faces_index])
                {
                    last_vert_pos = n_face->vert_[faces_index];
                }
                break;
            case 2:
                n_face->vert_t[faces_index] = stoi(temp.str());break;
            case 3:
                n_face->vert_n[faces_index] = stoi(temp.str());break;
            default:;
        }
        // cout << "face index: " << faces_index << "val: " << n_face->val[faces_index] << endl;
        temp.str("");
        temp.clear();
        faces_index++;
    }
    if(n_face != nullptr)
    {
        // printf("vert_size: %d\r\n", last_vert_pos);
        n_face->vert_size = faces_index;
        faces.push_back(n_face);
    }
    return 0;
}
int parser_init()
{
    memset(&dimension, 0, sizeof(dimension));
    init_dim_x= 0;
    init_dim_y= 0;
    init_dim_z= 0;
    line_num = 0;
    found=0;
    object_index=-1;
    vert_size=0;
    t_dim = nullptr;
    return 0;
}

int update_obj_list()
{
    // printf("%f, %f\r\n", dimension.x_min, dimension.x_max);
    init_dim_x=init_dim_y=init_dim_z=0;
//    printf("%f, %f\r\n", dimension.x_min, dimension.x_max);
    dimension.id = object_index;
    dimension.name = obj_name;
    dimension.vert_size = vert_size;
    t_dim = new obj_info();
    memcpy(t_dim, &dimension, sizeof(dimension));
    object_list.emplace(std::make_pair(object_index, t_dim), faces);
    memset(&dimension, 0, sizeof(dimension));
    //object_list[object_index] = faces;
    obj_name.clear();
    faces.clear();

    return 0;
}

int parse_line(string line)
{
    line_num++;
    //std::cout << "LINE NUM " <<  line_num << std::endl;
//    LOGE("Line: %s\n", line.c_str());
    if(!line.size())
    {
        if(object_index!= -1)
        {
            update_obj_list();
        }
        //std::cout << "empty" << std::endl;
        LOGE("File is EMPTY\n");
        return 0;
    }

    found = line.find("o");
    if(found != string::npos)
    {
        if(line.at(found+1) != ' ')
        {
            return 0;//continue;
        }
        if(object_index!= -1)
        {
            update_obj_list();
        }

        obj_name = line.substr(found+2);
        object_index++;
        cout << line << endl;
        return 0;//continue;
    }

    found = line.find("f");
    if(found != string::npos)
    {
        parse_indices(line);
        return 0;//continue;
    }

    found = line.find("vt");
    if(found != string::npos)
    {
        parse_vertices(line ,"vt");
        return 0;//continue;;
    }

    found = line.find("vn");
    if(found != string::npos)
    {
        parse_vertices(line ,"vn");
        return 0;//continue;;
    }

    found = line.find("v");
    if(found != string::npos)
    {
        if(line.at(found+1) == ' ')
        {
            ++vert_size;
            parse_vertices(line ,"v");
            return 0;//continue;;
        }
    }
    return 0;
}

int parse(ifstream& in_file)
{
    //stringstream ctx(str_file);
#if 0    
    size_t found = ctx.find("f");
    while(found != string::npos)
    {
        cout << found << endl;
        found = ctx.find("f", found+1);
        for(std::string::iterator it = ctx.)
        {

        }
        face *n_face = new face;
        n_face->a = 
    }
#endif
    std::string line;
    while(!in_file.eof())
    {
        getline(in_file, line);
        parse_line(line);
    }
    return 0;
}

void parseObj(const char* fileName)
{
    ifstream inFile;
    stringstream str_test;

    //inFile.open(fileName);
    FILE *file = fopen(fileName, "rb");
    if(file == NULL)
    {
        LOGE("Cannot read file '%s'\n", fileName);
    }
    else{
        LOGE("file read successfully '%s'\n", fileName);
    }
    /*
    if(inFile.is_open())
    {
        std::string temp;
        parse(inFile);
        //print_s();
        LOGE("File open, %s", fileName);
    }
    else
    {
        std::cout << "Invalid FILE" << endl;
        LOGE("File open, %s cannt open", fileName);
    }

    inFile.close();
     */
//    return 0;
}

int prepare_vertices()
{
    update_obj_list();
    update_vertices();
    update_indices();
    return 0;
}
#if 0
int main(int argc, char** argv)
{
    int ret=0;

    if(argc <=1 || argc >=3)
    {
        std::cout << "./parser filename" << endl;
    }
    else
    {
        std::cout << argv[1] << endl;
    }

    ifstream inFile;
    stringstream str_test;

    inFile.open(argv[1]);

    if(inFile.is_open())
    {
        std::string temp;
        // while(!inFile.eof())
        // {
        //     getline(inFile, temp);
        //     temp.append("\n");
        //     str_test << temp;
        // }
        parse(inFile);
        // parse_vertices(inFile, "v");
        // parse_vertices(inFile, "vn");
        // parse_vertices(inFile, "vt");
        //print_vertices();
        //print_faces();
        print_s();
        //cout << str_test.str();
    }
    else
    {
        std::cout << "Invalid FILE" << endl;    
    }
    inFile.close();

    std::cout << "completed" << endl;

    return 0;
}
#endif
extern "C"{
    JNIEXPORT void JNICALL
    Java_com_ahmed_fun_1gl_NativeLib_parse(JNIEnv *env, jclass clazz, jstring fileName) {
        jboolean copyBytes;
        parseObj(env->GetStringUTFChars(fileName, &copyBytes));
    }

    JNIEXPORT void JNICALL
    Java_com_ahmed_fun_1gl_NativeLib_parseInit(JNIEnv *env, jclass clazz) {
        parser_init();
    }

    JNIEXPORT void JNICALL
    Java_com_ahmed_fun_1gl_NativeLib_parseLine(JNIEnv *env, jclass clazz, jstring line) {
        jboolean copyBytes;
        parse_line(env->GetStringUTFChars(line, &copyBytes));
    }

    JNIEXPORT void JNICALL
    Java_com_ahmed_fun_1gl_NativeLib_prepareVertices(JNIEnv *env, jclass clazz) {
        prepare_vertices();
    }
}