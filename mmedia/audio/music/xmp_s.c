
#include <xmp.h>
#include "core/mmedia/audio/music/xmp_s.h"
#include "core/mmedia/audio/music/xmp_common.h"

int get_sequence2(struct context_data *ctx, int ord)
{
	struct player_data *p = &ctx->p;
	return p->sequence_control[ord];
}

static void set_position2(struct context_data *ctx, int pos, int dir, int diff_time)
{
	struct player_data *p = &ctx->p;
	struct module_data *m = &ctx->m;
	struct xmp_module *mod = &m->mod;
	struct flow_control *f = (struct flow_control *)&p->flow;
	int seq, start;

	/* If dir is 0, we can jump to a different sequence */
	if (dir == 0) {
		seq = get_sequence2(ctx, pos);
	} else {
		seq = p->sequence;
	}



	if (seq == 0xff)
		return;

	start = m->seq_data[seq].entry_point;

	if (seq >= 0) {
		p->sequence = seq;

		if (pos >= 0) {
			int pat;

			while (mod->xxo[pos] == 0xfe) {
				if (dir < 0) {
					if (pos > start) {
						pos--;
					}
				} else {
					pos++;
				}
			}



			pat = mod->xxo[pos];



			if (pat < mod->pat) {
				if (pat == 0xff) {
					return;
				}

			//	printf("pos:%i ord:%i c:%i\n",pos,p->scan[seq].ord,pos > p->scan[seq].ord);

				if (pos > p->scan[seq].ord) {
					f->end_point = 0;
				} else {
					f->num_rows = mod->xxp[pat]->rows;
					f->end_point = p->scan[seq].num;
					f->jumpline = 0;
				}

				//p->row=30;
				//p->pos=30;
				//f->jump=60;
			}
		}

		if (pos < mod->len) {

			// let's suppose classic mode ...
			int row_duration = (2500*2)/p->bpm;

			//int num_rows = f->num_rows;
			int estimated_row = diff_time/row_duration;

		//	printf("pos < len %i %i diff time to set %i r:%i %i\n",pos, mod->len,diff_time,estimated_row,f->end_point);


			if (pos == 0) { //restart
				if(estimated_row<=1)
					p->pos =-1;
				else
					p->pos = 0;

			} else {
				p->pos = pos;
			}

			if(pos != -1 && estimated_row > 1){
				printf("set row!\n");
				p->row = estimated_row;
				//f->jumpline = estimated_row;
			}
		}
	}
}

int xmp_seek_time2(xmp_context opaque, int time)
{
	struct context_data *ctx = (struct context_data *)opaque;
	struct player_data *p = &ctx->p;
	struct module_data *m = &ctx->m;
	int i, t;//, r=1;//=(rand()%64)+1;


	if (ctx->state < XMP_STATE_PLAYING)
		return -XMP_ERROR_STATE;

	for (i = m->mod.len - 1; i >= 0; i--) {
		int pat = m->mod.xxo[i];
		if (pat >= m->mod.pat) {
			continue;
		}
		if (get_sequence2(ctx, i) != p->sequence) {
			continue;
		}
		t = m->xxo_info[i].time;
		if (time >= t) {

			set_position2(ctx, i, 1,time-t);
			break;
		}
	}
	if (i < 0) {
		printf("\nreset!");
		xmp_set_position(opaque, 0);
	}

	return p->pos < 0 ? 0 : p->pos;
}


