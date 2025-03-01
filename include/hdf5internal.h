/* Copyright 2018-2018 University Corporation for Atmospheric
   Research/Unidata. */
/**
 * @file This header file contains macros, types, and prototypes for
 * the HDF5 code in libhdf5. This header should not be included in
 * code outside libhdf5.
 *
 * @author Ed Hartnett
*/

#ifndef _HDF5INTERNAL_
#define _HDF5INTERNAL_

#include "config.h"
#include <hdf5.h>
#include <hdf5_hl.h>
#include "nc4internal.h"
#include "ncdimscale.h"
#include "nc4dispatch.h"
#include "hdf5dispatch.h"

#define NC_MAX_HDF5_NAME (NC_MAX_NAME + 10)

/* These have to do with creating chuncked datasets in HDF5. */
#define NC_HDF5_UNLIMITED_DIMSIZE (0)
#define NC_HDF5_CHUNKSIZE_FACTOR (10)
#define NC_HDF5_MIN_CHUNK_SIZE (2)

#define NC_EMPTY_SCALE "NC_EMPTY_SCALE"

/* This is an attribute I had to add to handle multidimensional
 * coordinate variables. */
#define COORDINATES "_Netcdf4Coordinates"
#define COORDINATES_LEN (NC_MAX_NAME * 5)

/* This is used when the user defines a non-coordinate variable with
 * same name as a dimension. */
#define NON_COORD_PREPEND "_nc4_non_coord_"

/* An attribute in the HDF5 root group of this name means that the
 * file must follow strict netCDF classic format rules. */
#define NC3_STRICT_ATT_NAME "_nc3_strict"

/* If this attribute is present on a dimscale variable, use the value
 * as the netCDF dimid. */
#define NC_DIMID_ATT_NAME "_Netcdf4Dimid"

/** This is the name of the class HDF5 dimension scale attribute. */
#define HDF5_DIMSCALE_CLASS_ATT_NAME "CLASS"

/** This is the name of the name HDF5 dimension scale attribute. */
#define HDF5_DIMSCALE_NAME_ATT_NAME "NAME"

/** Strut to hold HDF5-specific info for the file. */
typedef struct  NC_HDF5_FILE_INFO
{
   hid_t hdfid;
} NC_HDF5_FILE_INFO_T;

/* This is a struct to handle the dim metadata. */
typedef struct NC_HDF5_DIM_INFO
{
   hid_t hdf_dimscaleid;        /* Non-zero if a DIM_WITHOUT_VARIABLE dataset is in use (no coord var). */
   HDF5_OBJID_T hdf5_objid;
} NC_HDF5_DIM_INFO_T;

/** Strut to hold HDF5-specific info for attributes. */
typedef struct  NC_HDF5_ATT_INFO
{
   hid_t native_hdf_typeid;     /* Native HDF5 datatype for attribute's data */
} NC_HDF5_ATT_INFO_T;

/* Struct to hold HDF5-specific info for a group. */
typedef struct NC_HDF5_GRP_INFO
{
   hid_t hdf_grpid;
} NC_HDF5_GRP_INFO_T;

/* Struct to hold HDF5-specific info for a variable. */
typedef struct NC_HDF5_VAR_INFO
{
   hid_t hdf_datasetid;
   HDF5_OBJID_T *dimscale_hdf5_objids;
} NC_HDF5_VAR_INFO_T;

/* Struct to hold HDF5-specific info for a field. */
typedef struct NC_HDF5_FIELD_INFO
{
   hid_t hdf_typeid;
   hid_t native_hdf_typeid;
} NC_HDF5_FIELD_INFO_T;

/* Struct to hold HDF5-specific info for a type. */
typedef struct NC_HDF5_TYPE_INFO
{
   hid_t hdf_typeid;
   hid_t native_hdf_typeid;
} NC_HDF5_TYPE_INFO_T;

/* Logging and debugging. */
void reportopenobjects(int log, hid_t);
int hdf5_set_log_level();

/* These functions deal with HDF5 dimension scales. */
int rec_detach_scales(NC_GRP_INFO_T *grp, int dimid, hid_t dimscaleid);
int rec_reattach_scales(NC_GRP_INFO_T *grp, int dimid, hid_t dimscaleid);
int delete_dimscale_dataset(NC_GRP_INFO_T *grp, int dimid, NC_DIM_INFO_T *dim);

/* Write metadata. */
int nc4_rec_write_metadata(NC_GRP_INFO_T *grp, nc_bool_t bad_coord_order);
int nc4_rec_write_groups_types(NC_GRP_INFO_T *grp);

/* Adjust the cache. */
int nc4_adjust_var_cache(NC_GRP_INFO_T *grp, NC_VAR_INFO_T * var);

/* Open a HDF5 dataset. */
int nc4_open_var_grp2(NC_GRP_INFO_T *grp, int varid, hid_t *dataset);

/* Find types. */
NC_TYPE_INFO_T *nc4_rec_find_hdf_type(NC_FILE_INFO_T* h5,
                                      hid_t target_hdf_typeid);
int nc4_get_hdf_typeid(NC_FILE_INFO_T *h5, nc_type xtype,
		       hid_t *hdf_typeid, int endianness);

/* Enddef and closing files. */
int nc4_close_hdf5_file(NC_FILE_INFO_T *h5, int abort, NC_memio *memio);
int nc4_rec_grp_HDF5_del(NC_GRP_INFO_T *grp);
int nc4_enddef_netcdf4_file(NC_FILE_INFO_T *h5);

/* Break & reform coordinate variables */
int nc4_break_coord_var(NC_GRP_INFO_T *grp, NC_VAR_INFO_T *coord_var, NC_DIM_INFO_T *dim);
int nc4_reform_coord_var(NC_GRP_INFO_T *grp, NC_VAR_INFO_T *coord_var, NC_DIM_INFO_T *dim);

/* In-memory functions */
extern hid_t NC4_image_init(NC_FILE_INFO_T* h5);
extern void NC4_image_finalize(void*);

/* These functions are internal to the libhdf5 directory. */
int nc4_get_fill_value(NC_FILE_INFO_T *h5, NC_VAR_INFO_T *var, void **fillp);


/* Provenance Management (moved from nc4internal.h) */
/* Initialize the fileinfo global state */
extern int NC4_provenance_init();

/* Finalize the fileinfo global state */
extern int NC4_provenance_finalize();

/* Extract the provenance from a file, using dfalt as default */
extern int NC4_get_provenance(NC_FILE_INFO_T* file, const char* propstring, const struct NCPROPINFO* dfalt);

/* Set the provenance for a created file using dfalt as default */
extern int NC4_set_provenance(NC_FILE_INFO_T* file, const struct NCPROPINFO* dfalt);

/* Recover memory of an NCPROVENANCE object */
extern int NC4_free_provenance(struct NCPROVENANCE* prov);

extern int NC4_hdf5get_libversion(unsigned*,unsigned*,unsigned*);/*libsrc4/nc4hdf.c*/
extern int NC4_hdf5get_superblock(struct NC_FILE_INFO*, int*);/*libsrc4/nc4hdf.c*/
extern int NC4_isnetcdf4(struct NC_FILE_INFO*); /*libsrc4/nc4hdf.c*/

/* Convert a NCPROPINFO instance to a single string. */
extern int NC4_buildpropinfo(struct NCPROPINFO* info, char** propdatap);

/* Use HDF5 API to read the _NCProperties attribute */
extern int NC4_read_ncproperties(NC_FILE_INFO_T*);

/* Use HDF5 API to write the _NCProperties attribute */
extern int NC4_write_ncproperties(NC_FILE_INFO_T*);

/* Find file, group, var, and att info, doing lazy reads if needed. */
int nc4_hdf5_find_grp_var_att(int ncid, int varid, const char *name, int attnum,
                              int use_name, char *norm_name, NC_FILE_INFO_T **h5,
                              NC_GRP_INFO_T **grp, NC_VAR_INFO_T **var,
                              NC_ATT_INFO_T **att);

/* Find var, doing lazy var metadata read if needed. */
int nc4_hdf5_find_grp_h5_var(int ncid, int varid, NC_FILE_INFO_T **h5,
                             NC_GRP_INFO_T **grp, NC_VAR_INFO_T **var);

/* Perform lazy read of the rest of the metadata for a var. */
int nc4_get_var_meta(NC_VAR_INFO_T *var);

#endif /* _HDF5INTERNAL_ */
