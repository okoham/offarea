#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>


typedef struct 
{
    double x;
    double y;
    double z;
} 
point;


// könnte das auch einfacher machen: pro zeile ein array mit malloc
// länge des arrays ist ja bekannt
typedef struct vertex
{
    int id;
    struct vertex *next;
} 
vertex;


// das geht auch ...
typedef struct 
{
    int nv;
    int *vertices;
} 
face;


// triangle area
double dummy_triangle_area(point v0, point v1, point v2)
{
    return 0.1234;
}


double triangle_area(point v0, point v1, point v2)
{
    double tz = (v1.x * v0.y) - (v0.x * v1.y) 
             + (v2.x * v1.y) - (v1.x * v2.y)
             + (v0.x * v2.y) - (v2.x * v0.y);
    double ty = (v1.x * v0.z) - (v0.x * v1.z) 
             + (v2.x * v1.z) - (v1.x * v2.z)
             + (v0.x * v2.z) - (v2.x * v0.z);
    double tx = (v1.y * v0.z) - (v0.y * v1.z) 
             + (v2.y * v1.z) - (v1.y * v2.z)
             + (v0.y * v2.z) - (v2.y * v0.z);
    return 0.5 * sqrt(tx*tx + ty*ty + tz*tz);
}



void parse_line2_tok(char *line, int *nv, int *nf)
{
    char *delim = " ";
    *nv = atoi(strtok(line, delim));
    *nf = atoi(strtok(NULL, delim));
}


// get the x, y, z coordinates
void parse_coords(char *line, point *p)
{
    // char *token = NULL;
    char *delim = " ";
    p->x = atof(strtok(line, delim)); 
    p->y = atof(strtok(NULL, delim)); 
    p->z = atof(strtok(NULL, delim)); 
}

// parse the vertices into an array? 
void parse_face(char *line, face *f)
{
    char *delim = "\n ";
    size_t size = sizeof(int);

    // number of vertices
    char *token = strtok(line, delim);
    int nv = atoi(token);

    // allocate memory
    int *vertex_indices = calloc(size, nv);
    if (vertex_indices == NULL)
    {
        exit(1);
    }

    // loop over remaining items
    int *p = vertex_indices;
    for (int i = 0; i < nv; i++, p += sizeof(int))
    {
        token = strtok(NULL, delim);
        *p = atoi(token);
    }

    // build struct
    f->nv = nv;
    f->vertices = vertex_indices;
}


// parse the vertices into a linked list
// here, we ignore the provided information of the number of vertices.
vertex *parse_face_list(char* line)
{
    // space and newline
    char *delim = "\n ";
    // initialise list
    vertex *list = NULL;
    // first element is num vertices. not used.
    char *token = strtok(line, delim);

    // this stores the vertexes in revesed order.
    while ((token = strtok(NULL, delim)) != NULL)
    {
        vertex *v = malloc(sizeof(vertex));
        if (v == NULL)
        {
            exit(1);
        }
        v->id = atoi(token);
        v->next = list;
        list = v;
    }
    return list;
}


// free vertex lists. recursively
void free_list(vertex *list)
{
    if (list != NULL)
    {
        free_list(list->next);
        free(list);
    }
}


int main(int argc, char *argv[])
{

    // TODO: --help

    // check number of arguments == 1
    if (argc != 2)
    {
        printf("Usage: offarea INFILE\n");
        return 1;
    }

    // open file. return false if not available
    FILE *infile = fopen(argv[1], "r");
    if (infile == NULL)
    {
        printf("File %s not found, exiting.\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // allocate memory for word
    char *line = NULL;
    unsigned long len = 0;
    long nread;

    // first line: should be OFF
    nread = getline(&line, &len, infile);
    if (strcasecmp(line, "OFF\n") != 0)
    {
        printf("Not an OFF file.\n");
        exit(EXIT_FAILURE);     
    }

    int nv = 0;
    int nf = 0;

    // nv, nf, ne
    if ((nread = getline(&line, &len, infile)) != -1)
    {
        parse_line2_tok(line, &nv, &nf);
        printf("%i, %i\n", nv, nf);
    }

    // allocate memory for vertex coordinates
    point *vertices = calloc(nv, sizeof(point));

    // read vertices
    for (int i = 0; i < nv; i++)
    {
        nread = getline(&line, &len, infile);
        point *p = &vertices[i];
        parse_coords(line, p);
        printf("%f %f %f\n", p->x, p->y, p->z);        
    }
    
    // faces
    /*
    vertex *faces[nf];
    for (int i = 0; i < nf; i++)
    {
        nread = getline(&line, &len, infile);
        faces[i] = parse_face_list(line);

        // debug
        vertex *tmp = faces[i];
        while (tmp != NULL)
        {
            printf("%i ", tmp->id);
            tmp = tmp->next;
        }
        printf("\n");
    }
    // free memory
    for (int i = 0; i < nf; i++)
    {
        free_list(faces[i]);
    }    
    */

    // faces, other way
    face faces[nf];
    face *f = &faces[0];
    for (int i = 0; i < nf; i++, f++)
    {
        nread = getline(&line, &len, infile);
        parse_face(line, f);

        // debug
        printf("%i (%i): ", i, f->nv);
        int *nid = f->vertices;
        for (int j = 0; j < f->nv; j++, nid += sizeof(int))
        {
            // int idx = *(f->vertices + j*(sizeof(int)));
            // printf("%i ", idx);
            printf("%i ", *nid);
        }
        printf("\n");
    }

    // free memory
    for (int i = 0; i < nf; i++)
    {
        free(faces[i].vertices);
    }



    
    // close file
    fclose(infile);

    free(line);
    free(vertices);


    // write a function that frees the linked list
    

    // read first line
    // make sure it starts with OFF and two numbers
    // handle freak cases with missing newline

    // allocate memory for arrays

    // loop over nodes
    // return error if not proper format
    // return error if not expected number of lines

    // loop over elements
    // return error if incorrect format or number of lines not meeting expectations

    // compute area
    // print area

    // free memory

    return 0;
}