#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h> // For close()
#include "/Users/rudyan/Documents/GitHub/OwnWebServer/C-Simple-JSON-Parser/json.h"

#define PORT 8080  

int main() {
    int client_fd = 0, n = 0;
	char recv_buff[1024];
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;

    memset(&address, 0, sizeof(address)); 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = htonl(INADDR_ANY); 
    address.sin_port = htons(PORT);
     
    if (client_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (connect(client_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

	while ( (n = read(client_fd, recv_buff, sizeof(recv_buff)-1)) > 0)
	{
		recv_buff[n] = '\0'; // Додайте символ завершення рядка
		if(fputs(recv_buff, stdout) == EOF)
		{
			printf("\n Error : Fputs error\n");
		}
	}

    if(n < 0)
	{
		printf("\n Read error \n");
	}

    char json_start = strstr(recv_buff, "{");
        printf("%c", json_start);
    if (json_start == NULL) {
        fprintf(stderr, "JSON data not found in response\n");
        return 1;
    }
    // Передача JSON рядка до функції парсингу JSON
    result(json_element) element_result = json_parse(json_start );
    // Обробка результатів парсингу
    if (result_is_err(json_element)(&element_result)) {
        typed(json_error) error = result_unwrap_err(json_element)(&element_result);
        fprintf(stderr, "Error parsing JSON: %s\n", json_error_to_string(error));
        return -1;
    }

    // Отримання значення елемента "hello" з об'єкта JSON
    typed(json_element) element = result_unwrap(json_element)(&element_result);
    result(json_element) hello_element_result = json_object_find(element.value.as_object, "hello");
    if (result_is_err(json_element)(&hello_element_result)) {
        typed(json_error) error = result_unwrap_err(json_element)(&hello_element_result);
        fprintf(stderr, "Error getting element \"hello\": %s\n", json_error_to_string(error));
        return -1;
    }

    // Виведення значення елемента "hello" на екран
    typed(json_element) hello_element = result_unwrap(json_element)(&hello_element_result);
    printf("%s\n", hello_element.value.as_string);

    close(client_fd);
    return 0; 
}
