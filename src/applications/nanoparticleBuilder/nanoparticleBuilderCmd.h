/* nanoparticleBuilderCmd.h */

/* File autogenerated by gengetopt version 2.15  */

#ifndef NANOPARTICLEBUILDERCMD_H
#define NANOPARTICLEBUILDERCMD_H

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef CMDLINE_PARSER_PACKAGE
#define CMDLINE_PARSER_PACKAGE "nanoparticleBuilder"
#endif

#ifndef CMDLINE_PARSER_VERSION
#define CMDLINE_PARSER_VERSION "1.0"
#endif

struct gengetopt_args_info
{
  char * output_arg;	/* Output file name.  */
  char * output_orig;	/* Output file name original value given at command line.  */
  double latticeCnst_arg;	/* Lattice spacing in Angstroms for cubic lattice..  */
  char * latticeCnst_orig;	/* Lattice spacing in Angstroms for cubic lattice. original value given at command line.  */
  double radius_arg;	/* Nanoparticle radius in Angstroms.  */
  char * radius_orig;	/* Nanoparticle radius in Angstroms original value given at command line.  */
  double* ShellRadius_arg;	/* Radius from the center of the particle to start a new shell in multi-component core-shell particle. Specified for each component > 1 in md file..  */
  char ** ShellRadius_orig;	/* Radius from the center of the particle to start a new shell in multi-component core-shell particle. Specified for each component > 1 in md file. original value given at command line.  */
  double* molFraction_arg;	/* (Default) Builds a multi-component random mixed nanoparticle. Mole Fraction must be specified for each componet > 1 in MD file..  */
  char ** molFraction_orig;	/* (Default) Builds a multi-component random mixed nanoparticle. Mole Fraction must be specified for each componet > 1 in MD file. original value given at command line.  */
  double vacancyPCT_arg;	/* Percentage of vacancies to build.  */
  char * vacancyPCT_orig;	/* Percentage of vacancies to build original value given at command line.  */
  double* vacancyRadius_arg;	/* Radius arround core-shell where vacancies should be located..  */
  char ** vacancyRadius_orig;	/* Radius arround core-shell where vacancies should be located. original value given at command line.  */
  
  int help_given ;	/* Whether help was given.  */
  int version_given ;	/* Whether version was given.  */
  int output_given ;	/* Whether output was given.  */
  int latticeCnst_given ;	/* Whether latticeCnst was given.  */
  int radius_given ;	/* Whether radius was given.  */
  unsigned int ShellRadius_given ;	/* Whether ShellRadius was given.  */
  unsigned int molFraction_given ;	/* Whether molFraction was given.  */
  int vacancyPCT_given ;	/* Whether vacancyPCT was given.  */
  unsigned int vacancyRadius_given ;	/* Whether vacancyRadius was given.  */

  char **inputs ; /* unamed options */
  unsigned inputs_num ; /* unamed options number */
} ;

int cmdline_parser (int argc, char * const *argv, struct gengetopt_args_info *args_info);
int cmdline_parser2 (int argc, char * const *argv, struct gengetopt_args_info *args_info, int override, int initialize, int check_required);
int cmdline_parser_file_save(const char *filename, struct gengetopt_args_info *args_info);

void cmdline_parser_print_help(void);
void cmdline_parser_print_version(void);

void cmdline_parser_init (struct gengetopt_args_info *args_info);
void cmdline_parser_free (struct gengetopt_args_info *args_info);

int cmdline_parser_required (struct gengetopt_args_info *args_info, const char *prog_name);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* NANOPARTICLEBUILDERCMD_H */
