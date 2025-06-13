#ifndef __Single_magnetic_crystal__
#define __Single_magnetic_crystal__

// standard includes
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstddef>
struct Transform { int i; };


// share block




  /* used for reading data table from file */
  %include "read_table-lib"
  %include "interoff-lib"




//  %include "columnfile"
/* Declare structures and functions only once in each instrument. */
#ifndef SINGLE_MAGNETIC_CRYSTAL_DECL
#define SINGLE_MAGNETIC_CRYSTAL_DECL
#include <complex.h>
    struct hkl_data
    {
      int h,k,l;                  /* Indices for this reflection */
      double F2;                  /* Value of structure factor */
      complex double f[4];        /* Structure factors (scattering amplitudes for different spin flips spin up->up, down->down, up->down, down-up */
      double tau_x, tau_y, tau_z; /* Coordinates in reciprocal space */
      double tau;                 /* Length of (tau_x, tau_y, tau_z) */
      double u1x, u1y, u1z;       /* First axis of local coordinate system */
      double u2x, u2y, u2z;       /* Second axis of local coordinate system */
      double u3x, u3y, u3z;       /* Third axis of local coordinate system */
      double sig1, sig2, sig3;    /* RMSs of Gauss axis */
      double sig123;              /* The product sig1*sig2*sig3 */
      double m1, m2, m3;          /* Diagonal matrix representation of Gauss */
      double cutoff;              /* Cutoff value for Gaussian tails */
    };

  struct hkl_info_struct
    {
      struct hkl_data *list;      /* Reflection array */
      int count;                  /* Number of reflections */
      struct tau_data *tau_list;  /* Reflections close to Ewald Sphere */
      double m_delta_d_d;         /* Delta-d/d FWHM */
      double m_ax,m_ay,m_az;      /* First unit cell axis (direct space, AA) */
      double m_bx,m_by,m_bz;      /* Second unit cell axis */
      double m_cx,m_cy,m_cz;      /* Third unit cell axis */
      double asx,asy,asz;         /* First reciprocal lattice axis (1/AA) */
      double bsx,bsy,bsz;         /* Second reciprocal lattice axis */
      double csx,csy,csz;         /* Third reciprocal lattice axis */
      double m_a, m_b, m_c;       /* lattice parameter lengths */
      double m_aa, m_bb, m_cc;    /* lattice angles */
      double sigma_a, sigma_i;    /* abs and inc X sect */
      double rho;                 /* density */
      double at_weight;           /* atomic weight */
      double at_nb;               /* nb of atoms in a cell */
      double V0;                  /* Unit cell volume (AA**3) */
      int    column_order[5];     /* column signification [h,k,l,F,F2] */
      int    recip;               /* Flag to indicate if recip or direct cell axes given */
      int    shape;               /* 0:cylinder, 1:box, 2:sphere 3:any shape*/
      int    flag_warning;        /* number of warnings */
      double tau_max;
      double tau_min;
      double refx,refy,refz;      /*chosen polarisation reference direction*/
    };

  struct tau_data
    {
      int index;                  /* Index into reflection table */
      double refl;
      double xsect;
      double sigma_1, sigma_2;
      /* The following vectors are in local koordinates. */
      double kix, kiy, kiz;       /* Initial wave vector */
      double rho_x, rho_y, rho_z; /* The vector ki - tau */
      double rho;                 /* Length of rho vector */
      double ox, oy, oz;          /* Origin of Ewald sphere tangent plane */
      double nx, ny, nz;          /* Normal vector of Ewald sphere tangent */
      double b1x, b1y, b1z;       /* Spanning vectors of Ewald sphere tangent */
      double b2x, b2y, b2z;
      double l11, l12, l22;       /* Cholesky decomposition L of 2D Gauss */
      double det_L;               /* Determinant of L */
      double y0x, y0y;            /* 2D Gauss center in tangent plane */
    };

  int
  read_hkl_data(char *atoms_file, struct hkl_info_struct *info,
    double SC_mosaic, double SC_mosaic_h, double SC_mosaic_v, double SC_mosaic_n)
  {/*{{{*/
    struct hkl_data *list = NULL;
    int size = 0;
    t_Table sTable; /* sample data table structure from atoms_file */
    int i=0;
    double tmp_x, tmp_y, tmp_z;
    char **parsing;
    char flag=0;
    
    double nb_atoms=1;
    t_Table atoms;

    if (!atoms_file || !strlen(atoms_file) || !strcmp(atoms_file,"NULL") || !strcmp(atoms_file,"0" ) ) {
      info->count = 0;
      flag=1;
    }
    if (!flag) {
        int status;
      if( (status=Table_Read(&atoms,atoms_file,0))==-1){
        fprintf(stderr,"Single_magnetic_crystal: Error reading atom list from file: %s\n",atoms_file);return(0);
      }
      
      printf("index type x y z b\n");
      for (i=0;i<atoms.rows;i++){
        printf("atom %d",(int)floor(Table_Index(atoms,i,0)));
        int j;
        for (j=1;j<atoms.columns;j++){
            printf(" %g",Table_Index(atoms,i,j));
        }
        printf("\n"); 
      }

      /* Compute reciprocal or direct lattice vectors. */
      if (!info->recip) {/*{{{*/
        vec_prod(tmp_x, tmp_y, tmp_z,
            info->m_bx, info->m_by, info->m_bz,
            info->m_cx, info->m_cy, info->m_cz);
        info->V0 = fabs(scalar_prod(info->m_ax, info->m_ay, info->m_az, tmp_x, tmp_y, tmp_z));
        printf("V0=%g\n", info->V0);

        info->asx = 2*PI/info->V0*tmp_x;
        info->asy = 2*PI/info->V0*tmp_y;
        info->asz = 2*PI/info->V0*tmp_z;
        vec_prod(tmp_x, tmp_y, tmp_z, info->m_cx, info->m_cy, info->m_cz, info->m_ax, info->m_ay, info->m_az);
        info->bsx = 2*PI/info->V0*tmp_x;
        info->bsy = 2*PI/info->V0*tmp_y;
        info->bsz = 2*PI/info->V0*tmp_z;
        vec_prod(tmp_x, tmp_y, tmp_z, info->m_ax, info->m_ay, info->m_az, info->m_bx, info->m_by, info->m_bz);
        info->csx = 2*PI/info->V0*tmp_x;
        info->csy = 2*PI/info->V0*tmp_y;
        info->csz = 2*PI/info->V0*tmp_z;
      } else {
        info->asx = info->m_ax;
        info->asy = info->m_ay;
        info->asz = info->m_az;
        info->bsx = info->m_bx;
        info->bsy = info->m_by;
        info->bsz = info->m_bz;
        info->csx = info->m_cx;
        info->csy = info->m_cy;
        info->csz = info->m_cz;

        vec_prod(tmp_x, tmp_y, tmp_z,
            info->bsx/(2*PI), info->bsy/(2*PI), info->bsz/(2*PI),
            info->csx/(2*PI), info->csy/(2*PI), info->csz/(2*PI));
        info->V0 = 1/fabs(scalar_prod(info->asx/(2*PI), info->asy/(2*PI), info->asz/(2*PI), tmp_x, tmp_y, tmp_z));
        printf("V0=%g\n", info->V0);

        info->asx = 2*PI/info->V0*tmp_x;
        info->asy = 2*PI/info->V0*tmp_y;
        info->asz = 2*PI/info->V0*tmp_z;
        vec_prod(tmp_x, tmp_y, tmp_z, info->m_cx, info->m_cy, info->m_cz, info->m_ax, info->m_ay, info->m_az);
        info->bsx = 2*PI/info->V0*tmp_x;
        info->bsy = 2*PI/info->V0*tmp_y;
        info->bsz = 2*PI/info->V0*tmp_z;
        vec_prod(tmp_x, tmp_y, tmp_z, info->m_ax, info->m_ay, info->m_az, info->m_bx, info->m_by, info->m_bz);
        info->csx = 2*PI/info->V0*tmp_x;
        info->csy = 2*PI/info->V0*tmp_y;
        info->csz = 2*PI/info->V0*tmp_z;
      }/*}}}*/
      /*store lattice vector lengths for later reference*/
      info->m_a=sqrt(scalar_prod(info->m_ax,info->m_ay,info->m_az,info->m_ax,info->m_ay,info->m_az));
      info->m_b=sqrt(scalar_prod(info->m_bx,info->m_by,info->m_bz,info->m_bx,info->m_by,info->m_bz));
      info->m_c=sqrt(scalar_prod(info->m_cx,info->m_cy,info->m_cz,info->m_cx,info->m_cy,info->m_cz));

      /*give the atom list - calculate the chemical structure factors of a series of hkls that are within the q-range*/
      double as,bs,cs;
      as=sqrt(scalar_prod(info->asx,info->asy,info->asz,info->asx,info->asy,info->asz));
      bs=sqrt(scalar_prod(info->bsx,info->bsy,info->bsz,info->bsx,info->bsy,info->bsz));
      cs=sqrt(scalar_prod(info->csx,info->csy,info->csz,info->csx,info->csy,info->csz));

      complex double f=0;
      if (info->tau_max==-1)
        info->tau_max=4*as;
      
      double q;
      int h,k,l,m;
    
      /* allocate hkl_data array initially make room for 2048 reflections. will be expanded (realloc'd) if needed.*/
      size=2048;
      list = (struct hkl_data*)malloc(size*sizeof(struct hkl_data));

      printf("q=[%g %g]\n",info->tau_min,info->tau_max);
      printf("as,bs,cs=(%g %g %g)\n",as,bs,cs);
      i=0;
      for (h=-floor(info->tau_max/as);h<ceil(info->tau_max/as);h++){
        for (k=-floor(info->tau_max/bs);k<ceil(info->tau_max/bs);k++){
          for (l=-floor(info->tau_max/cs);l<ceil(info->tau_max/cs);l++){
            if (h==0 && k==0 && l==0) continue;
            double qx= (h*info->asx+k*info->bsx+l*info->csx);
            double qy= (h*info->asy+k*info->bsy+l*info->csy);
            double qz= (h*info->asz+k*info->bsz+l*info->csz);
            q=sqrt(qx*qx+qy*qy+qz*qz);
            if (q<info->tau_min || q>info->tau_max) continue;
            f=0;
            complex double f_tau=0;
            complex double f_[4]={0,0,0,0};
            for (m=0;m<atoms.rows;m++){
              /*Tabulated values are usually in fm. Divide by 10 to end up with x-sections in barns*/
              double b=Table_Index(atoms,m,5)/10;
              f_tau = cexp(I*2*M_PI*(h*Table_Index(atoms,m,2) + k*Table_Index(atoms,m,3) + l*Table_Index(atoms,m,4)));
              //printf("%g b=%g r=(%g %g %g) hkl=(%2d %2d %2d), exp(-i*2pi*(H.r)=(%g%+gj)\n",atoms.data[m][0],b,atoms.data[m][2],atoms.data[m][3],atoms.data[m][4],h,k,l,creal(f_tau),cimag(f_tau));
              if(atoms.columns>6){
                /*the atom has a magnetic moment*/
                double S_x,S_y,S_z,L_x,L_y,L_z;
                double i_x=0,i_y=0,i_z=0;
                /*G. Williams Definition of r0 / cm =-gamma*e^2/(m_e c^2)*/
                const double r0=-0.5391e-12;
                double g,gs,gl;
                double M,beta=0;
                gs=Table_Index(atoms,m,6);
                /*S_a,S_b,S_c in the file are given in crystal coordinates - so convert that to the crystal cartesian coordinate system*/ 
                S_x=Table_Index(atoms,m,7)*info->m_ax/info->m_a+Table_Index(atoms,m,8)*info->m_bx/info->m_b+ Table_Index(atoms,m,9)*info->m_cx/info->m_c;
                S_y=Table_Index(atoms,m,7)*info->m_ay/info->m_a+Table_Index(atoms,m,8)*info->m_by/info->m_b+ Table_Index(atoms,m,9)*info->m_cy/info->m_c;
                S_z=Table_Index(atoms,m,7)*info->m_az/info->m_a+Table_Index(atoms,m,8)*info->m_bz/info->m_b+ Table_Index(atoms,m,9)*info->m_cz/info->m_c;
                gl=Table_Index(atoms,m,10);
                /*L_a,L_b,L_c in the file are given in crystal coordinates - so convert that to the crystal cartesian coordinate system*/ 
                L_x=Table_Index(atoms,m,11)*info->m_ax/info->m_a+Table_Index(atoms,m,12)*info->m_bx/info->m_b+ Table_Index(atoms,m,13)*info->m_cx/info->m_c;
                L_y=Table_Index(atoms,m,11)*info->m_ay/info->m_a+Table_Index(atoms,m,12)*info->m_by/info->m_b+ Table_Index(atoms,m,13)*info->m_cy/info->m_c;
                L_z=Table_Index(atoms,m,11)*info->m_az/info->m_a+Table_Index(atoms,m,12)*info->m_bz/info->m_b+ Table_Index(atoms,m,13)*info->m_cz/info->m_c;
                g=gs+gl;

                M=fabs(r0)*g/2*1e12;/*2nd factor to end up with x-sections in barns*/
                if ( (S_x!=0 || S_y!=0 || S_z!=0) ){
                  /*S_|_=q_norm x (S x _norm) */
                  double S_orto_x,S_orto_y,S_orto_z;
                  vec_prod(S_orto_x,S_orto_y,S_orto_z,S_x,S_y,S_z,qx,qy,qz);
                  vec_prod(S_orto_x,S_orto_y,S_orto_z,qx,qy,qz,S_x,S_y,S_z);
                  S_orto_x/=(q*q);S_orto_y/=(q*q);S_orto_z/=(q*q);

                  /* Refer to coordinates where polarisation is along z*/
                  double mm1sq=scalar_prod(info->refx,info->refy,info->refz,info->refx,info->refy,info->refz);
                  /*gram-schmidt orthogonalization*/
                  double mm2x=1,mm2y=0,mm2z=0,mm2sq=1,mm3x=0,mm3y=0,mm3z=1,mm3sq=1;
                  if (info->refy==0){
                    if (info->refx==0 ){mm2x=1;mm2y=0;mm2z=0; mm3x=0;mm3y=1;mm3z=0;}
                    else {mm2x=0;mm2y=1;mm2z=0; mm3x=0;mm3y=0;mm3z=1;}
                  }
                  double tmpx,tmpy,tmpz;
                  tmpx= mm2x-scalar_prod(mm2x,mm2y,mm2z,info->refx,info->refy,info->refz)/mm1sq*info->refx;
                  tmpy= mm2y-scalar_prod(mm2x,mm2y,mm2z,info->refx,info->refy,info->refz)/mm1sq*info->refy;
                  tmpz= mm2z-scalar_prod(mm2x,mm2y,mm2z,info->refx,info->refy,info->refz)/mm1sq*info->refz;
                  mm2x=tmpx;mm2y=tmpy;mm2z=tmpz;
                  mm2sq=scalar_prod(mm2x,mm2y,mm2z,mm2x,mm2y,mm2z);

                  tmpx= mm3x-scalar_prod(mm3x,mm3y,mm3z,info->refx,info->refy,info->refz)/mm1sq*info->refx;
                  tmpy= mm3y-scalar_prod(mm3x,mm3y,mm3z,info->refx,info->refy,info->refz)/mm1sq*info->refy;
                  tmpz= mm3z-scalar_prod(mm3x,mm3y,mm3z,info->refx,info->refy,info->refz)/mm1sq*info->refz;
                  mm3x=tmpx;mm3y=tmpy;mm3z=tmpz;
                  tmpx= mm3x-scalar_prod(mm3x,mm3y,mm3z,mm2x,mm2y,mm2z)/mm2sq*info->refx;
                  tmpy= mm3y-scalar_prod(mm3x,mm3y,mm3z,mm2x,mm2y,mm2z)/mm2sq*info->refy;
                  tmpz= mm3z-scalar_prod(mm3x,mm3y,mm3z,mm2x,mm2y,mm2z)/mm2sq*info->refz;
                  mm3x=tmpx;mm3y=tmpy;mm3z=tmpz;

                  S_x=scalar_prod(S_orto_x,S_orto_y,S_orto_z,mm2x,mm2y,mm2z);
                  S_y=scalar_prod(S_orto_x,S_orto_y,S_orto_z,mm3x,mm3y,mm3z);
                  S_z=scalar_prod(S_orto_x,S_orto_y,S_orto_z,info->refx,info->refy,info->refz);
                }
		f_[0]+=f_tau*( b -M*S_z + (beta/2)*i_z );
                f_[1]+=f_tau*( b +M*S_z - (beta/2)*i_z );
                f_[2]+=f_tau*( -M*(S_x + I*S_y) + (beta/2)*(i_x + I*i_y) );
                f_[3]+=f_tau*( -M*(S_x - I*S_y) + (beta/2)*(i_x - I*i_y) );
              }else{
                /*scattering is non-magnetic*/
                f_[0]+=b*f_tau;
                f_[1]+=b*f_tau;
                f_[2]+=0;
                f_[3]+=0;
              }                
            }
            if (cabs(f_[0])>FLT_EPSILON || cabs(f_[1])>FLT_EPSILON || cabs(f_[2])>FLT_EPSILON || cabs(f_[3])>FLT_EPSILON ){
              list[i].h=h;
              list[i].k=k;
              list[i].l=l;
              for (m=0;m<4;m++){
                list[i].f[m]=f_[m];
              } 
              list[i].F2=cabs(f_tau)*cabs(f_tau);
              if(++i==size){
                size=2*size;
                list = (struct hkl_data*)realloc(list,size*sizeof(struct hkl_data));
              }
              printf("(hkl)=(%2d %2d %2d) ",h,k,l);
              printf(" |f++|^2=%g |f--|^2=%g |f+-|^2=%g |f-+|^2=%g",cabs(f_[0])*cabs(f_[0]),cabs(f_[1])*cabs(f_[1]),cabs(f_[2])*cabs(f_[2]),cabs(f_[3])*cabs(f_[3]));
              printf(" f++=%g%+gj, f--=%g%+gj, f+-=%g%+gj, f-+=%g%+gj\n",creal(f_[0]),cimag(f_[0]),creal(f_[1]),cimag(f_[1]),creal(f_[2]),cimag(f_[2]),creal(f_[3]),cimag(f_[3]) );
            }
          }
        }
      }
      /*set size to actual number of reflections*/
      size=i;
    }

    if (flag) return(-1);

    /*re-loop over reflections and evaluate mosaicity coefficients etc.*/
    for (i=0; i<size; i++)
    {
      double b1[3], b2[3];
      /* Precompute some values */
      list[i].tau_x = list[i].h*info->asx + list[i].k*info->bsx + list[i].l*info->csx;
      list[i].tau_y = list[i].h*info->asy + list[i].k*info->bsy + list[i].l*info->csy;
      list[i].tau_z = list[i].h*info->asz + list[i].k*info->bsz + list[i].l*info->csz;
      list[i].tau = sqrt(list[i].tau_x*list[i].tau_x + list[i].tau_y*list[i].tau_y +list[i].tau_z*list[i].tau_z);
      list[i].u1x = list[i].tau_x/list[i].tau;
      list[i].u1y = list[i].tau_y/list[i].tau;
      list[i].u1z = list[i].tau_z/list[i].tau;
      list[i].sig1 = FWHM2RMS*info->m_delta_d_d*list[i].tau;
      /* Find two arbitrary axes perpendicular to tau and each other. */
      normal_vec(&b1[0], &b1[1], &b1[2],
                 list[i].u1x, list[i].u1y, list[i].u1z);
      vec_prod(b2[0], b2[1], b2[2],
               list[i].u1x, list[i].u1y, list[i].u1z,
               b1[0], b1[1], b1[2]);
      /* Find the two mosaic axes perpendicular to tau. */
      if(SC_mosaic > 0) {
        /* Use isotropic mosaic. */
        list[i].u2x = b1[0];
        list[i].u2y = b1[1];
        list[i].u2z = b1[2];
        list[i].sig2 = FWHM2RMS*list[i].tau*MIN2RAD*SC_mosaic;
        list[i].u3x = b2[0];
        list[i].u3y = b2[1];
        list[i].u3z = b2[2];
        list[i].sig3 = FWHM2RMS*list[i].tau*MIN2RAD*SC_mosaic;
      } else  {
        /* Use anisotropic mosaic. */
        /*This is not implemeted fully yet (see todo below)- therefore exit with a warning*/ 
        fprintf(stderr,"Single_magnetic_crystal: Anisotropic mosaic not implemented yet - aborting.\n");
        return(0);
      }
      list[i].sig123 = list[i].sig1*list[i].sig2*list[i].sig3;
      list[i].m1 = 1/(2*list[i].sig1*list[i].sig1);
      list[i].m2 = 1/(2*list[i].sig2*list[i].sig2);
      list[i].m3 = 1/(2*list[i].sig3*list[i].sig3);
      /* Set Gauss cutoff to 5 times the maximal sigma. */
      if(list[i].sig1 > list[i].sig2)
        if(list[i].sig1 > list[i].sig3)
          list[i].cutoff = 5*list[i].sig1;
        else
          list[i].cutoff = 5*list[i].sig3;
      else
        if(list[i].sig2 > list[i].sig3)
          list[i].cutoff = 5*list[i].sig2;
        else
          list[i].cutoff = 5*list[i].sig3;
    }
    info->list = list;
    info->count = i;
    info->tau_list = malloc(i*sizeof(*info->tau_list));
    if(!info->tau_list)
    {
      fprintf(stderr, "Single_crystal: Error: Out of memory!\n");
      return(0);
    }
    return(info->count = i);
  }/*}}}*/
#endif /* !SINGLE_MAGNETIC_CRYSTAL_DECL */


struct Single_magnetic_crystal {
    Transform t;
    Transform *parent;

    // parameters
    string atom_sites = 0;
    vector geometry = NULL;
    float xwidth = 0;
    float yheight = 0;
    float zdepth = 0;
    float radius = 0;
    float delta_d_d = 1e-4;
    float mosaic = -1;
    float mosaic_h = -1;
    float mosaic_v = -1;
    float mosaic_n = -1;
    float recip_cell = 0;
    float q_min = 0;
    float q_max = -1;
    float mx = 0;
    float my = 1;
    float mz = 0;
    float na = 1;
    float nb = 1;
    float nc = 1;
    float ax = 0;
    float ay = 0;
    float az = 0;
    float bx = 0;
    float by = 0;
    float bz = 0;
    float cx = 0;
    float cy = 0;
    float cz = 0;
    float p_transmit = -1;
    float sigma_abs = 0;
    float sigma_inc = 0;
    float order = 0;

    // declares
    double d = 1e-15;
    float f = 0.0;
    float f2 = 1.1f;
    int i = 0;
    char *i = "a_string_value";
    char *c = NULL;
    struct hkl_info_struct hkl_info;
    off_struct offdata;
};

Single_magnetic_crystal Init_Single_magnetic_crystal() {
    Single_magnetic_crystal _comp = {};
    Single_magnetic_crystal *comp = &_comp;

    #define atom_sites comp->atom_sites
    #define geometry comp->geometry
    #define xwidth comp->xwidth
    #define yheight comp->yheight
    #define zdepth comp->zdepth
    #define radius comp->radius
    #define delta_d_d comp->delta_d_d
    #define mosaic comp->mosaic
    #define mosaic_h comp->mosaic_h
    #define mosaic_v comp->mosaic_v
    #define mosaic_n comp->mosaic_n
    #define recip_cell comp->recip_cell
    #define q_min comp->q_min
    #define q_max comp->q_max
    #define mx comp->mx
    #define my comp->my
    #define mz comp->mz
    #define na comp->na
    #define nb comp->nb
    #define nc comp->nc
    #define ax comp->ax
    #define ay comp->ay
    #define az comp->az
    #define bx comp->bx
    #define by comp->by
    #define bz comp->bz
    #define cx comp->cx
    #define cy comp->cy
    #define cz comp->cz
    #define p_transmit comp->p_transmit
    #define sigma_abs comp->sigma_abs
    #define sigma_inc comp->sigma_inc
    #define order comp->order

    #define d comp->d
    #define f comp->f
    #define f2 comp->f2
    #define i comp->i
    #define i comp->i
    #define c comp->c
    #define hkl_info comp->hkl_info
    #define offdata comp->offdata
    // ---------------------------


  double as, bs, cs;

  /* transfer input parameters */
  hkl_info.m_delta_d_d = delta_d_d;
  hkl_info.m_ax = na*ax;
  hkl_info.m_ay = na*ay;
  hkl_info.m_az = na*az;
  hkl_info.m_bx = nb*bx;
  hkl_info.m_by = nb*by;
  hkl_info.m_bz = nb*bz;
  hkl_info.m_cx = nc*cx;
  hkl_info.m_cy = nc*cy;
  hkl_info.m_cz = nc*cz;
  hkl_info.sigma_a = sigma_abs;
  hkl_info.sigma_i = sigma_inc;
  hkl_info.recip = recip_cell;
  hkl_info.tau_min=q_min;
  hkl_info.tau_max=q_max;
  if (mx!=0 || my!=0 || mz!=0){
    hkl_info.refx=mx;
    hkl_info.refy=my;
    hkl_info.refz=mz;
    NORM(hkl_info.refx,hkl_info.refy,hkl_info.refz);
  }else{
    hkl_info.refx=hkl_info.refz=0;hkl_info.refy=1;
  }
  /* Read in structure factors, and do some pre-calculations. */
  if (!read_hkl_data(atom_sites, &hkl_info, mosaic, mosaic_h, mosaic_v, mosaic_n))
    exit(-1);
  if (hkl_info.count)
    printf("Single_crystal: %s: Read %d reflections from file '%s'\n",
        NAME_CURRENT_COMP, hkl_info.count, atom_sites);
  else printf("Single_crystal: %s: Using incoherent elastic scattering only.\n",
      NAME_CURRENT_COMP, hkl_info.sigma_i);
      
  hkl_info.shape=-1; /* -1:no shape, 0:cyl, 1:box, 2:sphere, 3:any-shape  */
  if (geometry && strlen(geometry) && strcmp(geometry, "NULL") && strcmp(geometry, "0")) {
	  if (off_init(geometry, xwidth, yheight, zdepth, 0, &offdata)) {
      hkl_info.shape=3; 
    }
  }
  else if (xwidth && yheight && zdepth)  hkl_info.shape=1; /* box */
  else if (radius > 0 && yheight)        hkl_info.shape=0; /* cylinder */
  else if (radius > 0 && !yheight)       hkl_info.shape=2; /* sphere */

  if (hkl_info.shape < 0) 
    exit(fprintf(stderr,"Single_magnetic_crystal: %s: sample has invalid dimensions.\n"
                        "ERROR           Please check parameter values (xwidth, yheight, zdepth, radius).\n", NAME_CURRENT_COMP));
  
  printf("Single_magnetic_crystal: %s: Vc=%g [Angs] sigma_abs=%g [barn] sigma_inc=%g [barn] reflections=%s\n",
      NAME_CURRENT_COMP, hkl_info.V0, hkl_info.sigma_a, hkl_info.sigma_i, atom_sites && strlen(atom_sites) ? atom_sites : "NULL");

  printf("WARNING; Single_magnetic_crystal has not yet been experimentally validated. Please use caution when intepreting results.\n");


    // ---------------------------
    #undef atom_sites
    #undef geometry
    #undef xwidth
    #undef yheight
    #undef zdepth
    #undef radius
    #undef delta_d_d
    #undef mosaic
    #undef mosaic_h
    #undef mosaic_v
    #undef mosaic_n
    #undef recip_cell
    #undef q_min
    #undef q_max
    #undef mx
    #undef my
    #undef mz
    #undef na
    #undef nb
    #undef nc
    #undef ax
    #undef ay
    #undef az
    #undef bx
    #undef by
    #undef bz
    #undef cx
    #undef cy
    #undef cz
    #undef p_transmit
    #undef sigma_abs
    #undef sigma_inc
    #undef order

    #undef d
    #undef f
    #undef f2
    #undef i
    #undef i
    #undef c
    #undef hkl_info
    #undef offdata

    return _comp;
}
#endif
