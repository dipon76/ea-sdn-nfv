/*
 * Copyright (c) 2018, Toshiba Research Europe Ltd.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */
/**
 * \file
 *         Atom SDN Controller: Energy Aware routing application.
 * \author
 *         Dipon Saha
 */
#include "net/ip/uip.h"

#include "net/sdn/sdn.h"

#include "atom.h"

/* Log configuration */
#include "sys/log-ng.h"
#define LOG_MODULE "ATOM"
#define LOG_LEVEL LOG_LEVEL_ATOM

/* Search */
static atom_node_t *stack[ATOM_MAX_NODES];  /* Stack for searching */
static int top = -1;                  /* Keeps track of the stack index */

/* Path */
static atom_node_t *spath[ATOM_MAX_NODES];  /* Keeps track of shortest path */
static int spath_length = ATOM_MAX_NODES;  /* Assume worst case */

static int state = 0;
static int nfv[] = {5, 8, 19};
static int tx_node[] = {7, 17, 29};
extern static int prilist[40][100] = {0};
extern static int seclist[40][100] = {0};
extern static int nfv-map [40] = {0};
static int res[3][40] = {0};
static int capacity[40] = {0};


static int get_id(int node_id){
  int i;
  for (i=0;i < 3 ;i++){
    if (node_id == tx_node[i]){
      return i;
    }
  }
  return -1;
}

static int get_length(int array[]){

/* number of elements in `array` */
  int length = (int)(sizeof(array) / sizeof(int));
  return length;
}




/*---------------------------------------------------------------------------*/
/* Stack and Shortest Path */
/*---------------------------------------------------------------------------*/
static void push(atom_node_t *n) 
{
   stack[++top] = n;
}

static atom_node_t *pop() 
{
   return stack[top--];
}

static int size()
{
  return top+1;
}

static int
contains(int id) {
  int i;
  for (i = 0; i <= top; i++) {
    if(stack[i]->id == id) {
      return 1;
    }
  }
  return 0;
} 
static int taken (int id) {
  for (int i = 0; i <3 ; ++i)
  {
    /* code */
    for (int j = 0; j < 40; ++j)
    {
      /* code */
      if (res[i][j] == id) return 1;
      if (res[i][j] == 0) break;
    }
  }
  return 0;
}

void clear_spath(void) {
  int i;
  for(i = 0; i < ATOM_MAX_NODES; i++) {
    spath[i] = NULL;
  }
  spath_length = ATOM_MAX_NODES;
}

static void
copy_to_spath() {
  int i;
  for(i = 0; i<= top; i++) {
    spath[i] = stack[i];
  }
  spath_length = top+1;
}

static int* getAffected(){
  int result[40] = {0};
    for (int i = 0; i < 40; ++i)
    {
      /* code */
      for (int j = 0; j < 100; ++j)
      {
        /* code */
        if (routelist[i][j] == 0) break;
        node = atom_net_get_node_id(routelist[i][j])
        if(node.energy >= HIGHEST_ENERGY){
          result[i] = 1;
        }
      }
    }
    return result;
}

static int get_length(int* a){
  int i = 0;
  while(a[i] != NULL){
    i++;
  }
  return i;
}
/*---------------------------------------------------------------------------*/
static void
depth_first_search(atom_node_t *src, atom_node_t *dest)
{
  int i;
  atom_node_t *new_src;
  if(src != NULL && dest != NULL) {
    /* push to the search stack */
    push(src);
    /* Check if we have reached the destination */
    if(src->id == dest->id) {
      /* If this stack is the shortest we want to copy the path */
      if(size() <= spath_length) {
        clear_spath();
        copy_to_spath();
      }
      /* Pop the stack */
      pop(src);
      return;
    }
    /* If we havent reached the destination, then search the neighbours that
       haven't been visited */
    for(i = 0; i < src->num_links; i++) {
      if(!contains(src->links[i].dest_id)) {
        new_src = atom_net_get_node_id(src->links[i].dest_id);
        if(new_src != NULL) {
          depth_first_search(new_src, dest);
        } else {
          LOG_ERR("ERROR DFS could not find node!\n");
        }
      }
    }
    /* Pop the stack */
    pop(src);
  } else {
    LOG_ERR("ERROR depth_first_search SRC or DEST was NULL!\n");
  }
}

static void
depth_first_search2(atom_node_t *src, atom_node_t *dest)
{
  int i;
  atom_node_t *new_src;
  if(src != NULL && dest != NULL) {
    /* push to the search stack */
    push(src);
    /* Check if we have reached the destination */
    if(src->id == dest->id) {
      /* If this stack is the shortest we want to copy the path */
      if(size() <= spath_length) {
        clear_spath();
        copy_to_spath();
      }
      /* Pop the stack */
      pop(src);
      return;
    }
    /* If we havent reached the destination, then search the neighbours that
       haven't been visited */
    for(i = 0; i < src->num_links; i++) {
      if(!contains(src->links[i].dest_id) && !taken(src->links[i].dest_id)) {
        new_src = atom_net_get_node_id(src->links[i].dest_id);
        if(new_src != NULL) {
          depth_first_search(new_src, dest);
        } else {
          LOG_ERR("ERROR DFS could not find node!\n");
        }
      }
    }
    /* Pop the stack */
    pop(src);
  } else {
    LOG_ERR("ERROR depth_first_search SRC or DEST was NULL!\n");
  }
}


/*---------------------------------------------------------------------------*/
/* Application API */
/*---------------------------------------------------------------------------*/
static void
init(void) {
  LOG_INFO("Atom shortest path routing app initialised\n");
}
static int get_link_cost(int *list){
  int cost = 0;
  for (int i = 0; i < get_length(list); ++i)
  {
    atom_node_t src = atom_net_get_node_id(list[i]);
    for (int j = 0; j < src->num_links; ++j)
    {
      if (src->links[i].dest_id == list[i+1]){
        cost+= src->links[i].rssi;
        break;
      }
    }
  }
  return cost;
}

static void sort_2_routes(void){
  int cost1 = get_link_cost(res[0]);
  int cost2 = get_link_cost(res[1]); 
  int tem[40];
  if (cost2 < cost1){
    tem = res[0];
    res[0] = res[1];
    res[1] = tem;
  }
}

static int get_best(int *list){
  int len = get_length(list);
  int index = 0;
  int i;
  for( i = 1; i < len; i++)
  {
    if(list[i] < list[index])
        index = i;
    }

    return index;
}

static int get_total_energy(int *list){
  int len = get_length(list);
  int sum = 0; 
  for (int i = 0; i < len; ++i)
  {
    /* code */
    atom_node_t node = atom_net_get_node_id(list[i]);
    sum += node.energy;
  }
  return sum;
}

static void get_2_routes(void){
  int cost[3] = {0};
  int tem[40];
  cost[0] = get_total_energy(res[0]);
  cost[1] = get_total_energy(res[1]);
  cost[2] = get_total_energy(res[2]);
  if(cost[0] < cost[1] && cost[0] < cost[2])
  {
    continue;
  }
  else if(cost[1] < cost[2])
  {
    tem = res[0];
    res[0] = res[1];
    res[1] = tem;
    tem = cost[0];
    cost[0] = cost[1];
    cost[1] = tem;
  }
  else
  {
    tem = res[0];
    res[0] = res[2];
    res[2] = tem;
     tem = cost[0];
    cost[0] = cost[2];
    cost[2] = tem;
  } 
  
  if (cost[1] > cost[2]){
    tem = res[2];
    res[2]= res[1];
    res[1] = tem;
  }
   
}

static int* get_routes(atom_node_t *src, atom_node_t *dest){
  
  depth_first_search(src,dst);
  if(spath[0] != NULL) {
      /* Copy to route */
    for(i = 0; i < spath_length; i++){
        res[0][i] = spath[i]->id;
    }
  }
   depth_first_search2(src,dst);
  if(spath[0] != NULL) {
      /* Copy to route */
    for(i = 0; i < spath_length; i++){
        res[1][i] = spath[i]->id;
    }
  }
  depth_first_search2(src,dst);
  if(spath[0] != NULL) {
      /* Copy to route */
    for(i = 0; i < spath_length; i++){
        res[2][i] = spath[i]->id;
    }
  }
}

static void aps(int* txList, int *nfvList){
  int length = get_length(txList);
  int nfvLength = get_length(nfvList);

  for (int i = 0; i < length; ++i)
  {
    /* code */
    int primary[length];
    int seconday[length];
    int costNFV [nfvLength];
    for (int j = 0; j < nfvLength; ++j)
    {
      /* code */
      src = atom_net_get_node_id(txList[i]);
      dst = atom_net_get_node_id(nfvList[j]);
      get_routes(src,dst);
      get_2_routes(); 
      sort_2_route();
      atom_node_t nfv = atom_net_get_node_id(nfvList[j]);
      costNFV[j] = get_total_energy(res[0]) + nfv->activation; 
    }
    int nfv-id = get_best(costNFV);
    atom_node_t nfv = atom_net_get_node_id(nfvList[nfv-id]);
    while(nfv->capacity > = capacity[nfvList[nfv-id]]){
      costNFV[nfv-id] = 10000000;
      nfv-id = get_best(costNFV);
      nfv = atom_net_get_node_id(nfvList[nfv-id]);
    }
    nfv-capacity += 1;
    nfv-map[i] = nfvList[nfv-id]; 
    prilist[i] = res[0];
    seclist[i] = res[1];
  }
}

static void DRS(void){
  if(state == 0){
    aps(tx_node, nfv);
    state = 1;
  } else {
    tx_node = getAffected();
    aps(tx_node, nfv);
  }
}


/*---------------------------------------------------------------------------*/
static atom_response_t *
run(void *data)
{
  int i;
  sdn_srh_route_t route;
  sdn_srh_route_t route2;
  atom_node_t *src, *dest;

  /* Dereference the action data */
  atom_routing_action_t *action = (atom_routing_action_t *)data;



  route.length = get_length(prilist[src->id]);
  
  for(i = 0; i < route.length; i++) {
  route.nodes[i] = prilist[src->id][i];

  route2.length = get_length(seclist[src->id]);
  
  for(i = 0; i < route2.length; i++) {
  route2.nodes[i] = seclist[src->id][i];

    LOG_DBG("Found route from ");
    LOG_DBG_6ADDR(&action->src);
    LOG_DBG_(" to ");
    LOG_DBG_6ADDR(&action->dest);
    print_route(&route);
    LOG_DBG_("\n");
  // } else {
  //   LOG_ERR("ERROR No path between [%d] and [%d]! MAX_NODES=%d",
  //     src->id, dest->id, ATOM_MAX_NODES);
  //   return NULL;
  // }

  /* Return the response */
  atom_response_buf_copy_to(ATOM_RESPONSE_ROUTING, &route);
  atom_response_buf_copy_to(ATOM_RESPONSE_ROUTING, &route2);
  return ;
}

/*---------------------------------------------------------------------------*/
/* Application instance */
/*---------------------------------------------------------------------------*/
struct atom_app app_route_sp = {
  "SP Routing",
  ATOM_ACTION_ROUTING,
  init,
  run
};


