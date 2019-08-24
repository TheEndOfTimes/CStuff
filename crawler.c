
#include "crawler.h"



crawling(char* url, int hopsLimit){
  
 // long seed;

   int hopNum, numHops;

  //numHops = hopsLimit;

   char destAddr[MAX_ADDR_LENGTH];
   char startAddr[MAX_ADDR_LENGTH];

	struct listNode *pListStart;

	pListStart = malloc(sizeof(struct listNode));
	 if(pListStart == NULL){
    //	fprintf(stderr, "ERROR: could not allocate memory\n");
    	return -2;
	}
	strncpy(startAddr, url, MAX_ADDR_LENGTH);
  //startAddr[MAX_ADDR_LENGTH - 1] = '\0';
  //strncpy(pListStart->addr, startAddr, MAX_ADDR_LENGTH);
  pListStart->next = NULL;


  /* start the crawling */
  for(hopNum=1; hopNum <= numHops; hopNum++){
    int res = getLink(startAddr, destAddr, MAX_ADDR_LENGTH);

    if(!res){
    //  printf("Dead end on hop %d: no outgoing links\n", hopNum);
      break;
    }

    if(contains(pListStart, destAddr)){
     // printf("Cycle detected on hop %d: address %s\n", hopNum,
	     //destAddr);
      hopNum--; // try again for this hop in the next iteration
    }
    else{
      insertBack(pListStart, destAddr);
      strncpy(startAddr, destAddr, MAX_ADDR_LENGTH);
    }
  }

  //printAddresses(pListStart);

  destroyList(pListStart);

  return 0;
}


/*
 * returns 1 if the list starting at pNode contains the address addr,
 *    and returns 0 otherwise
 */
int contains(const struct listNode *pNode, const char *addr){
 //TODO: complete this
 	if(pNode==NULL){
			
				return 0;
			
			}else {
			
				if(strcmp(pNode->addr,addr)==0){
					return 1;
				}else 
				return contains(pNode->next,addr);
			}
}
    

/*
 * inserts the address addr as a new listNode at the end of
 *    the list
 */
void insertBack(struct listNode *pNode, const char *addr){
// TODO: complete this
 
 while(pNode->next!=NULL){
   
   
   pNode=pNode->next;
   
   
 }
 if(pNode->next==NULL){
		//printf("made to begginging of insertBack");
	struct listNode *temp;
	temp =  malloc(sizeof(struct listNode));
	strncpy(temp->addr,addr,strlen(addr)+1);
	pNode->next=temp;
	temp->next=NULL;
	//free(temp);
	//printf("made it to insertback");
	return ;
	} else{
	
		insertBack(pNode->next,addr);
	
}
	
 
}


/*
 * prints the addresses from pNode to the end of the list,
 *   one on each line
 */
void printAddresses(const struct listNode *pNode){
 //TODO: complete this
 	if(pNode==NULL){
	return ;
	}else{
		
		printf("%s \n",pNode->addr);
		return printAddresses(pNode->next);
	}
}

/*
 * frees the memory associated with this node and all subsequent nodes
 */
void destroyList(struct listNode *pNode){
 //TODO: complete this
 
 if(pNode==NULL){
   
  
   return;
 }else{
  
    struct listNode *temp;
   
	temp = pNode->next;
	free(pNode);
	destroyList(temp);
   
   
}
 
 
 
}
  


int getLink(const char* srcAddr, char* link, const int maxLinkLength){
  const int bufSize = 1000;
  char buffer[bufSize];

  int numLinks = 0;

  FILE *pipe;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python getLinks.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
	    buffer);
    return 0;
  }

  fscanf(pipe, "%d\n", &numLinks);

  if(numLinks > 0){
    int linkNum;
    double r = (double)rand() / ((double)RAND_MAX + 1.0);

    for(linkNum=0; linkNum<numLinks; linkNum++){
      fgets(buffer, bufSize, pipe);
      
      if(r < (linkNum + 1.0) / numLinks){
		break;
      }
    }

    /* copy the address from buffer to link */
    strncpy(link, buffer, maxLinkLength);
    link[maxLinkLength-1] = '\0';
    
    /* get rid of the newline */
    {
      char* pNewline = strchr(link, '\n');
      if(pNewline != NULL){
		*pNewline = '\0';
      }
    }
  }

  pclose(pipe);

  if(numLinks > 0){
    return 1;
  }
  else{
    return 0;
  }
}