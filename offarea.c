#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>


typedef struct vector
{
    double x;
    double y;
    double z;
} 
vector;


// das geht auch ...
typedef struct face
{
    int nv;
    int *vertices;
} 
face;


// function declarations
double face_area(face *f, vector points[]);
double triangle_area(vector v0, vector v1, vector v2);
void parse_line2_tok(char *line, int *nv, int *nf);
void parse_coords(char *line, vector *p);
void parse_face(char *line, face *f);
void print_coords(vector *coords, int n);
void print_faces(face *faces, int n);
void cross(vector *a, vector *b, vector *c);
void sum(vector *a, vector *b, vector *c);
double norm(vector *a);


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
    vector *vertices = calloc(nv, sizeof(vector));
    // read vertices
    for (int i = 0; i < nv; i++)
    {
        nread = getline(&line, &len, infile);
        parse_coords(line, &vertices[i]);     
    }
    // debug: print them
    // print_coords(vertices, nv);
    
    // faces, other way
    // TODO: assert that index is always < nv
    face faces[nf];
    for (int i = 0; i < nf; i++)
    {
        nread = getline(&line, &len, infile);
        parse_face(line, &faces[i]);
    }
    // debug: print them
    // print_faces(faces, nf);

    // close infile
    fclose(infile);
    free(line);

    // calculate areas
    double a = 0.0;
    for (int i = 0; i < nf; i++)
    {
        a += face_area(&faces[i], vertices);
    }
    printf("area: %f\n", a);

    // free memory
    free(vertices);
    for (int i = 0; i < nf; i++)
    {
        free(faces[i].vertices);
    }

    return 0;
}


// return face area
// 0,1,2 + 0,2,3 + 0,3,4 + 0,n-2,n-1
// n vertices; n-2 calculations
// compute area of a polygonal face
double face_area(face *f, vector points[])
{
    double area = 0.0;
    size_t offi = sizeof(int);  
    double da; 
    
    int nid0 = *f->vertices;
    for (int i = 0; i < f->nv - 2; i++)
    {
        int nid1 = *(f->vertices + (i + 1)*offi);
        int nid2 = *(f->vertices + (i + 2)*offi);
        da = triangle_area(points[nid0], points[nid1], points[nid2]);
        printf("%i, %i, %i: %f\n", nid0, nid1, nid2, da);
        area += da;
    }
    return area;
}


// ref scheider/eberly
// compute area of a triangle in 3d space
// TODO: reference
double triangle_area(vector v0, vector v1, vector v2)
{
    vector ab, bc, ca, tmp1, tmp2; 
    cross(&v0, &v1, &ab);
    cross(&v1, &v2, &bc);
    cross(&v2, &v0, &ca);
    sum(&ab, &bc, &tmp1);
    sum(&tmp1, &ca, &tmp2);
    return 0.5 * norm(&tmp2);
}


// parse line containing number of vertices, faces, edges.
// number of edges is ignored
void parse_line2_tok(char *line, int *nv, int *nf)
{
    char *delim = " ";
    *nv = atoi(strtok(line, delim));
    *nf = atoi(strtok(NULL, delim));
} 


// get the x, y, z coordinates
void parse_coords(char *line, vector *p)
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


// print vertex coordinates
void print_coords(vector *coords, int n)
{
    for (int i = 0; i < n; i++)
    {
        vector *p = &coords[i];
        printf("%f %f %f\n", p->x, p->y, p->z);   
    }
}


// print node indices for faces
void print_faces(face *faces, int nf)
{
    for (int i = 0; i < nf; i++)
    {
        face *f = &faces[i];
        printf("%i (%i): ", i, f->nv);
        int *nid = f->vertices;
        for (int j = 0; j < f->nv; j++, nid += sizeof(int))
        {
            printf("%i ", *nid);
        }
        printf("\n");
    }
}


// vector product of two 3d vectors
void cross(vector *a, vector *b, vector *c)
{
    c->x = (a->y * b->z) - (b->y * a->z);
    c->y = (a->z * b->x) - (b->z * a->x);
    c->z = (a->x * b->y) - (b->x * a->y);
}


// sum of two vectors
void sum(vector *a, vector *b, vector *c)
{
    c->x = a->x + b->x;
    c->y = a->y + b->y;
    c->z = a->z + b->z;
}


// 2-norm
double norm(vector *a)
{
    double sum = (a->x * a->x) + (a->y * a->y) + (a->z * a->z);
    return sqrt(sum);
}

