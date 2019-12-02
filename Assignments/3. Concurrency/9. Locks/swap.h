#ifndef SWAP_H
# define SWAP_H

int try( int *mutex) {
            return __sync_val_compare_and_swap(mutex, 0, 1);

} 
int lock( int *mutex)
{return spin;}

void unlock( int *mutex)