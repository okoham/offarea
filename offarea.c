#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>

#define DEBUG 0

// struct for vectors and points
typedef struct
{
    double x;
    double y;
    double z;
} 
vector;


// struct for faces, consisting of the number of indices and a
// pointer to the array of indices
typedef struct
{
    int nv;
    int *indices;
} 
face;


// function declarations
void parse_line2_tok(char *line, int *nv, int *nf);
void parse_coords(char *line, vector *p);
void parse_face(char *line, face *f);
void print_coords(vector *coords, int n);
void print_faces(face *faces, int n);
double triangle_area(vector v0, vector v1, vector v2);
double face_area(face *f, vector points[]);
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
        if (DEBUG)
            printf("%i, %i\n", nv, nf);
    }

    // allocate memory for vertex coordinates
    vector *coords = calloc(nv, sizeof(vector));
    // read vertex coordinates
    for (int i = 0; i < nv; i++)
    {
        nread = getline(&line, &len, infile);
        parse_coords(line, &coords[i]);     
    }
    
    if (DEBUG)
        print_coords(coords, nv);
    
    // faces, other way
    // TODO: assert that index is always < nv
    face *faces = calloc(nf, sizeof(face));
    for (int i = 0; i < nf; i++)
    {
        nread = getline(&line, &len, infile);
        // printf("%s", line);
        parse_face(line, &faces[i]);
    }
    if (DEBUG)
        print_faces(faces, nf);

    // close infile
    fclose(infile);
    free(line);

    // calculate area
    double a = 0.0;
    face *f = faces;
    for (int i = 0; i < nf; i++, f++)
    {
        a += face_area(f, coords);
    }
    printf("area: %f\n", a);

    // free memory
    free(coords);
    for (int i = 0; i < nf; i++)
    {
        free(faces[i].indices);
    }
    free(faces);

    // done
    return 0;    
}


// Calculate unsigned area of a convex (!!!) polygonal face.
// this decomposes the polygon into a triangle fan, which
// is valid for convex polygons only.
double face_area(face *f, vector points[])
{
    // get number of indices
    int nv = f->nv;
    // pointers to first three node ids
    int *indices = f->indices;
    int *nid0 = indices;
    int *nid1 = &indices[1]; 
    int *nid2 = &indices[2];

    // initialise area
    double area = 0.0;

    for (int i = 0; i < nv - 2; i++, nid1++, nid2++)
    {
        area += triangle_area(points[*nid0], points[*nid1], points[*nid2]);
        // DEBUG
        // printf("%i, %i, %i: %f  \n", *nid0, *nid1, *nid2, area);
    }
    return area;
}


// Compute area of a triangle in 3D space.
// Ref. Schneider, Eberly: Geometric Tools for Computer Graphics, 
// Morgan Kaufman 2003
double triangle_area(vector v0, vector v1, vector v2)
{
    vector ab, bc, ca, tmp1, tmp2; 
    cross(&v0, &v1, &ab);
    cross(&v1, &v2, &bc);
    cross(&v2, &v0, &ca);
    sum(&ab, &bc, &tmp1);
    sum(&tmp1, &ca, &tmp2);
    // FIXME: only for convex polygons ...
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
    char *delim = "\n ";
    p->x = atof(strtok(line, delim)); 
    p->y = atof(strtok(NULL, delim)); 
    p->z = atof(strtok(NULL, delim)); 
}


// parse the vertices into an array? 
void parse_face(char *line, face *f)
{
    char *delim = "\n ";

    // number of vertices for this polygon
    char *token = strtok(line, delim);
    int nvf = atoi(token);

    // allocate memory
    int *vertex_indices = calloc(nvf, sizeof(int));
    if (vertex_indices == NULL)
    {
        exit(1);
    }

    // loop over remaining items
    int *p = vertex_indices;
    for (int i = 0; i < nvf; i++, p++)
    {
        token = strtok(NULL, delim);
        *p = atoi(token);
    }

    // build struct
    f->nv = nvf;
    f->indices = vertex_indices;
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
    face *f = faces;
    for (int i = 0; i < nf; i++, f++)
    {
        printf("%i (%i): ", i, f->nv);
        int *nid = f->indices;
        for (int j = 0; j < f->nv; j++, nid++)
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


// 2-Norm of a vector
double norm(vector *a)
{
    double sum = (a->x * a->x) + (a->y * a->y) + (a->z * a->z);
    return sqrt(sum);
}

