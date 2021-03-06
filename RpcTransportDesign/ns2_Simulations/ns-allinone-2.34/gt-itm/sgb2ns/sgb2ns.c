/*
 * Copyright (c) 1997 by the University of Southern California
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation in source and binary forms for non-commercial purposes
 * and without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both the copyright notice and
 * this permission notice appear in supporting documentation. and that
 * any documentation, advertising materials, and other materials related
 * to such distribution and use acknowledge that the software was
 * developed by the University of Southern California, Information
 * Sciences Institute.  The name of the University may not be used to
 * endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THE UNIVERSITY OF SOUTHERN CALIFORNIA makes no representations about
 * the suitability of this software for any purpose.  THIS SOFTWARE IS
 * PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Other copyrights might apply to parts of this software and are so
 * noted when applicable.
 *
 * sgb2ns.c -- converter from sgb format to ns-2 configuration. 
 * $Id: sgb2ns.c,v 1.2 1998/07/20 21:49:26 haldar Exp $
 *
 * Daniel Zappala (daniel@isi.edu) wrote for converting GT-ITM's output to
 *   ns-1.
 * Polly Huang (phuang@catarina.usc.edu) and Haobo Yu (haoboy@isi.edu) 
 *   ported to ns-2.
 *
 * Maintainer: Haobo Yu (haoboy@isi.edu)
 *
 */

#include <stdio.h>
#include <strings.h>
#include "gb_graph.h"
#include "gb_save.h"
#include "geog.h"

main(argc,argv)
     int argc;
     char *argv[];
{
	int i, j, nlink;
	Vertex *v;
	Arc *a;
	Graph *g;
	FILE *fopen(), *fout;
	char m[420];

	if (argc != 3) {
		printf("sgb2ns <sgfile> <nsfile>\n\n");
		return;
	}
	fout = fopen(argv[2],"w");

	g = restore_graph(argv[1]);
	if (g == NULL) {
		printf("%s does not contain a correct SGB graph\n",argv[1]);
		return;
	}
			
	fprintf(fout, "# Generated by sgb2ns, created by Polly Huang\n");
	fprintf(fout, "# GRAPH (#nodes #edges id uu vv ww xx yy zz):\n");
	fprintf(fout, "# %d %d %s ",g->n, g->m, g->id);
	if (g->util_types[8] == 'I') fprintf(fout,"%ld ",g->uu.I);
	if (g->util_types[9] == 'I') fprintf(fout,"%ld ",g->vv.I);
	if (g->util_types[10] == 'I') fprintf(fout,"%ld ",g->ww.I);
	if (g->util_types[11] == 'I') fprintf(fout,"%ld ",g->xx.I);
	if (g->util_types[12] == 'I') fprintf(fout,"%ld ",g->yy.I);
	if (g->util_types[13] == 'I') fprintf(fout,"%ld ",g->zz.I);
	fprintf(fout,"\n\n");

	fprintf(fout, "proc create-topology {nsns node linkBW} {\n");
	fprintf(fout, "\tupvar $node n\n");
	fprintf(fout, "\tupvar $nsns ns\n\n");

	fprintf(fout,"\tset verbose 1\n\n");

	/* nodes */
	fprintf(fout, "\tif {$verbose} { \n");
	fprintf(fout, "\t\tputs \"creating nodes...\" \n");
	fprintf(fout, "}\n");
	fprintf(fout, "\t\tfor {set i 0} {$i < %d} {incr i} {\n", g->n);
	fprintf(fout, "\t\t\tset n($i) [$ns node]\n");
	fprintf(fout, "\t}\n");
	fprintf(fout, "\n");

	/* edges */
	fprintf(fout, "\t# EDGES (from-node to-node length a b):\n");
	nlink = 0;
	fprintf(fout, "\tif {$verbose} { \n");
	fprintf(fout, "\t\tputs -nonewline \"Creating links 0...\"\n");
	fprintf(fout, "\t\tflush stdout \n");
	fprintf(fout, "\t}\n");
	for (v = g->vertices,i=0; i < g->n; i++,v++) 
		for (a = v->arcs; a != NULL; a = a->next) {
			j = a->tip - g->vertices;
			if (j > i) {
				fprintf(fout, 
"\t$ns duplex-link-of-interfaces $n(%d) $n(%d) $linkBW %dms DropTail\n",
					i, j, 10 * a->len);
				nlink++;
				if ((nlink % 10) == 0) {
					fprintf(fout,
"\tif {$verbose} { puts -nonewline \"%d...\"; flush stdout }\n", 
						nlink);
				}
			}
		}
	fprintf(fout, "\n\tif {$verbose} { \n");
	fprintf(fout, "\t\tputs -nonewline \"%d...\"\n", nlink);
	fprintf(fout, "\t\tflush stdout\n");
	fprintf(fout, "\t\tputs \"starting\"\n");
	fprintf(fout, "\t}\n");

	/* srm members. join-group will be performed by Agent/SRM::start */
	/* return the number of nodes in this topology */
	fprintf(fout, "\treturn %d", g->n);
	fprintf(fout, "}\n");
}	

