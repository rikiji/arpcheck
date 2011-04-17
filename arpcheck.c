/*
 * arpcheck.c
 * Copyright (c) 2011 Riccardo Cecolin
 * http://rikiji.it - Sun Apr 17 10:47:39 CEST 2011
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/list.h>
#include <net/neighbour.h>
#include <linux/workqueue.h>

MODULE_LICENSE("GPL");

void arp_tbl_check(struct work_struct * w);

extern struct neigh_table arp_tbl;
#define HBUFFERLEN 30

struct neigh_list_t {
  unsigned char	ha[ALIGN(MAX_ADDR_LEN, sizeof(unsigned long))];
  u8 primary_key[0];
  struct list_head list;
} neigh_list;

static struct workqueue_struct * workq;
static DECLARE_DELAYED_WORK(work, arp_tbl_check);

void format_hwaddr(unsigned char * ha, char ha_len, char * hbuffer)
{
  int j, k;
  /* net/ipv4/arp.c */
  for (k = 0, j = 0; k < HBUFFERLEN - 3 && j < ha_len; j++) {
    hbuffer[k++] = hex_asc_hi(ha[j]);
    hbuffer[k++] = hex_asc_lo(ha[j]);
    hbuffer[k++] = ':';
  }
  if (k != 0)
    --k;
  hbuffer[k] = 0;  
}

void neigh_print(struct neighbour * n, void * null)
{
  char tbuf[16], hbuffer[HBUFFERLEN];
  struct net_device *dev = n->dev;  

  sprintf(tbuf, "%pI4", n->primary_key);
  format_hwaddr(n->ha, dev->addr_len, hbuffer);
  printk(KERN_ALERT "%-16s %s %s\n",tbuf,hbuffer, dev ? dev->name : "*");
}

void neigh_handler(struct neighbour * n, void * null)
{
  struct neigh_list_t *tmp;
  int found = 0;
  char hbuffer[HBUFFERLEN];

  /* search */
  list_for_each_entry(tmp, &neigh_list.list, list) {
    if(memcmp(n->ha,tmp->ha,n->dev->addr_len)==0) {
      format_hwaddr(n->ha, n->dev->addr_len, hbuffer);
      printk(KERN_ALERT "duplicated entry: %s\n", hbuffer);
      found = 1;
    }
  }
  
  /* add an entry */
  if(!found) {
    struct neigh_list_t * new_entry = (struct neigh_list_t *) kmalloc(sizeof(struct neigh_list_t), GFP_KERNEL);
    memcpy(new_entry->ha,n->ha,n->dev->addr_len);
    memcpy(new_entry->primary_key,n->primary_key,sizeof(u8 *));
    list_add(&(new_entry->list), &(neigh_list.list));
  }  
}

void arp_tbl_check(struct work_struct * w)
{
  struct list_head *pos, *q;
  struct neigh_list_t * tmp;

  neigh_for_each(&arp_tbl, neigh_handler, NULL);

  /* empty list */
  list_for_each_safe(pos, q, &neigh_list.list){
    tmp= list_entry(pos, struct neigh_list_t, list);
    list_del(pos);
    kfree(tmp);
  }
  queue_delayed_work(workq, &work, HZ * 5);
}

static int arp_init(void)
{
  INIT_LIST_HEAD(&neigh_list.list);
  printk(KERN_ALERT "arpcheck init\n");
  
  workq = create_singlethread_workqueue("arp_tbl_check_wq");
  queue_delayed_work(workq, &work, HZ * 5);
  return 0;
}

static void arp_exit(void)
{
  printk(KERN_ALERT "arpcheck exit\n");
  cancel_delayed_work(&work);
  flush_workqueue(workq);
  destroy_workqueue(workq);
}

module_init(arp_init);
module_exit(arp_exit);
