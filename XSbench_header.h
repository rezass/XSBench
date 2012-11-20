#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<math.h>
#include<omp.h>
#include<unistd.h>
#include<sys/time.h>

// Papi Definition (comment / uncomment to toggle PAPI)
//#define __PAPI

// Papi Header
#ifdef __PAPI
#include "/usr/local/include/papi.h"
#endif

// I/O Specifiers
#define INFO 1
#define DEBUG 1
#define SAVE 1
#define PRINT_PAPI_INFO 1

// For the numerical methods ran2() algorithm
#define IM1 2147483563
#define IM2 2147483399
#define AM (1.0/IM1)
#define IMM1 (IM1-1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define NTAB 32
#define NDIV (1+IMM1/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)


typedef struct{
	double energy;
	
	double total_xs;
	double elastic_xs;
	double absorbtion_xs;
	double fission_xs;
	double nu_fission_xs;
} NuclideGridPoint;

typedef struct{
	double energy;
	NuclideGridPoint ** xs_ptrs;
} GridPoint;

void logo(void);
void center_print(const char *s, int width);

NuclideGridPoint ** gpmatrix(size_t m, size_t n);

void gpmatrix_free( NuclideGridPoint ** M );

int NGP_compare( const void * a, const void * b );

void generate_grids( NuclideGridPoint ** nuclide_grids,
                     int n_isotopes, int n_gridpoints );

void sort_nuclide_grids( NuclideGridPoint ** nuclide_grids, int n_isotopes);

GridPoint * generate_energy_grid( int n_isotopes, int n_gridpoints,
                                  NuclideGridPoint ** nuclide_grids);

void set_grid_ptrs( GridPoint * energy_grid, NuclideGridPoint ** nuclide_grids,
                    int n_isotopes, int n_gridpoints );

NuclideGridPoint * binary_search( NuclideGridPoint * A, double quarry, int n );

void calculate_macro_xs( double p_energy, int mat, int n_isotopes,
                           int n_gridpoints, int * restrict num_nucs,
                           double ** restrict concs,
						   GridPoint * restrict energy_grid,
                           NuclideGridPoint ** restrict nuclide_grids,
						   int ** restrict mats,
                           double * restrict macro_xs_vector );

void calculate_micro_xs( int p_energy, int nuc, int n_isotopes,
                           int n_gridpoints,
                           GridPoint * restrict energy_grid,
                           NuclideGridPoint ** restrict nuclide_grids, int idx,
                           double * restrict xs_vector );

int grid_search( int n, double quarry, GridPoint * A);

int * load_num_nucs(void);
int ** load_mats( int * num_nucs );
double ** load_concs( int * num_nucs );
int pick_mat(unsigned long * seed);
double rn(unsigned long * seed);
float ran2( void );
void counter_stop( int * eventset, int num_papi_events );
void counter_init( int * eventset, int * num_papi_events );
