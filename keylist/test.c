#include <stdio.h>
#include "key_list.h"

#define LOG(format, ...) printf(format, ##__VA_ARGS__)

typedef struct _user {
    char *username;
} user_t;

static void test_value_releaser(value_t value)
{
    user_t* user = (user_t*) value.value;
    free(user);
}

int main(int argc,  char const *argv[]) 
{
    LOG("test key_list !\n");

    key_list_t* list = key_list_create(test_value_releaser);
    
    value_t v1, v2, v3;

    user_t* user1 = (user_t*) malloc(sizeof(user_t));
    user1->username = "jason";

    user_t* user2 = (user_t*) malloc(sizeof(user_t));
    user2->username = "jhuster";

    user_t* user3 = (user_t*) malloc(sizeof(user_t));
    user3->username = "ticktick";

    v1.value = user1;
    v2.value = user2;
    v3.value = user3;

    key_list_add(list, 101, v1);
    LOG("key_list_add %d,  %s\n", 101, ((user_t*)v1.value)->username);

    key_list_add(list, 102, v2);
    LOG("key_list_add %d,  %s\n", 102, ((user_t*)v2.value)->username);

    LOG("key list size = %d\n", key_list_count(list));

    LOG("Check key 102 is exist = %d\n", key_list_find_key(list, 102));
    LOG("Check key 103 is exist = %d\n", key_list_find_key(list, 103));

    key_list_foreach(list, node) {
        LOG("key_list_foreach key = %d,  value = %s \n", node->key, ((user_t*)node->value.value)->username);
    };

    key_t keys[2];
    if (key_list_keyset(list, keys, 2) < 0) {
        LOG("Failed to get key_list keyset ! \n");
        return -1;
    }

    int i=0;
    for (i=0; i<key_list_count(list); i++) {
        LOG("keyset : %d \n", keys[i]);
    }

    if (key_list_edit(list, 102, v3) == 0) {
        LOG("Update 102 success ! \n");        
    }    

    value_t value;    
    if (key_list_get(list, 102, &value) == 0) {
        LOG("Now the 102 value = %s \n", ((user_t*)value.value)->username);
    }

    if (key_list_delete(list, 102)==0) {
        LOG("key_list_delete 102 success ! \n");
    }

    LOG("key list size = %d\n", key_list_count(list));

    key_list_destroy(list);

    return 0;
}
