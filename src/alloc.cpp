//
// Created by jingzheng on 6/15/19.
//

#include <cstdlib>
#include "alloc.h"

char *alloc::start_free = 0;
char *alloc::end_free = 0;
size_t alloc::heap_size = 0;

alloc::obj *alloc::free_list[alloc::ENFreeLists::NFREELISTS] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

void *alloc::allocate(size_t bytes){

    if (bytes > EMaxBytes::MAXBYTES){//分配大于128字节的内存，直接分配返回
        printf("%sallocate size: %d greater than 128, use malloc\n", s4space.c_str(), bytes);
        return malloc(bytes);
    }
    size_t round_up = ROUND_UP(bytes);//输出round up的大小(这里没有意义仅仅输出查看)
    printf("allocate size: %d. after round up size is: %d \n", bytes, round_up);

    //定位bytes应该分配到那个链表中
    size_t index = FREELIST_INDEX(bytes);
    obj *list = free_list[index];
    if (list){//此list还有空间给我们
        printf("free_list has free node, index is: %d\n", index);
        free_list[index] = list->next;//使当前空闲节点指向下一个
        return list;
    }
    else{//此list没有足够的空间，需要从内存池里面取空间
        return refill(ROUND_UP(bytes));
    }
}


void alloc::deallocate(void *ptr, size_t bytes){
    if (bytes > EMaxBytes::MAXBYTES){//大于128的节点直接free释放
        free(ptr);
    }
    else{//小于128的节点重新分配给free_list(不释放内存)
        size_t index = FREELIST_INDEX(bytes);
        obj *node = static_cast<obj *>(ptr);
        node->next = free_list[index];
        free_list[index] = node;
    }
}


//返回一个大小为n的对象，并且有时候会为适当的free list增加节点
//假设bytes已经上调为8的倍数
void *alloc::refill(size_t bytes){
    size_t nobjs = ENObjs::NOBJS; // 默认为free_list bytes节点分配20个元素空间
    //从内存池里取，nobjs可能没有20个，因此参数是引用
    char *chunk = chunk_alloc(bytes, nobjs);
    obj **my_free_list = 0;
    obj *result = 0;
    obj *current_obj = 0, *next_obj = 0;
    printf("%srefill %d, objs\n", s4space.c_str(), nobjs);

    if (nobjs == 1){//取出的空间只够一个对象使用
        return chunk;
    }
    else{
        my_free_list = free_list + FREELIST_INDEX(bytes);
        result = (obj *)(chunk);//首个节点直接返回
//剩余的节点分配到对应的free_list中去
        *my_free_list = next_obj = (obj *)(chunk + bytes);
        //将取出的多余的空间加入到相应的free list里面去
        for (int i = 1;; ++i){
            current_obj = next_obj;
            next_obj = (obj *)((char *)next_obj + bytes);
            if (nobjs - 1 == i){
                current_obj->next = 0;//最后一个指针next 赋值null
                break;
            }
            else{
                current_obj->next = next_obj;
            }
        }
        return result;
    }
}

//假设bytes已经上调为8的倍数
char *alloc::chunk_alloc(size_t bytes, size_t& nobjs, int print_count){
    char *result = 0;
    size_t total_bytes = bytes * nobjs;
    size_t bytes_left = end_free - start_free;//当前有多少剩余的内存
    printf("%schunk_alloc round: %d,  need total_bytes: %d,  bytes_left: %d\n",
           s4space.c_str(), print_count, total_bytes, bytes_left);

    if (bytes_left >= total_bytes){//内存池剩余空间完全满足需要
        result = start_free;
        start_free = start_free + total_bytes;
        printf("%schunk_alloc round: %d, cater all, return %d objs bytes_left: %d >= total_bytes: %d\n",
               s8space.c_str(), print_count, nobjs, bytes_left, total_bytes);

        return result;
    }//内存池剩余空间不能完全满足需要，但足够供应一个或以上的区块
    else if (bytes_left >= bytes){
        nobjs = bytes_left / bytes;
        total_bytes = nobjs * bytes;

        result = start_free;
        start_free += total_bytes;
        printf("%schunk_alloc round: %d, cater part, return %d objs, size: %d return %d bytes\n",
               s8space.c_str(), print_count, nobjs, bytes, total_bytes);

        return result;
    }
    else{//内存池剩余空间连一个区块的大小都无法提供，重新申请
        size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
        printf("%schunk_alloc round: %d, malloc %d\n",
               s8space.c_str(), print_count, bytes_to_get);

        if (bytes_left > 0){//将原有的剩余的bytes分配给free_list
            obj **my_free_list = free_list + FREELIST_INDEX(bytes_left);
            ((obj *)start_free)->next = *my_free_list;
            *my_free_list = (obj *)start_free;
        }
// start_free 重新分配
        start_free = (char *)malloc(bytes_to_get);
        heap_size += bytes_to_get;
        end_free = start_free + bytes_to_get;//每次只会递归一次
        return chunk_alloc(bytes, nobjs, ++print_count);
    }
}


