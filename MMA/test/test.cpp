//
//  test.cpp
//  MMA
//
//  Created by Alex on 10/05/2020.
//  Copyright © 2020 郑嘉业. All rights reserved.
//

#include <stdio.h>
#include <cstdlib>
#define referenceString 300


int main(){
    
    int RS[referenceString];
    
    for(int i=0;i<referenceString;i++){
        
        RS[i]=rand()%30;
        
        
    }
    for(int i=0;i<referenceString;i++){
        
        printf("%d ",RS[i]);
        if((i+1)%6==0){
            printf("\n");
            
        }
    }
    return 0;
}
