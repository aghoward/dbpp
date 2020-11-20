# db++

`db++` is a c++ recreation of dirbuster (`dirb`). I made this a toy project after getting fed up with dirbuster being broken. This project will work in the happy path, but it is a toy so don't expect it to give nice failures yet. Feel free to submit PRs for anything you would like to see.

## Usage

```
Usage: ./db++ [-h|--help] [-w|--wordlist <wordlist_file>] [-r|--recursive] [-u|--username <username>] [-p|--password <password>] [-t|--request-templates <request_templates>] [-k|--ignore-ssl-errors] [-s|--ignored-status-codes <ignore_codes>] [-c|--ignored-content-lengths <ignore_content_lengths>] [-X|--request-method <request_method>] [-T|--thread-count <thread_count>] [-d|--data <request_data>] [-D|--content-type <content_type>] [-H|--headers <headers>] <base_url> 

    -h|--help                                              
                                                           Print this help message and exit                         
    -w|--wordlist <wordlist_file>                          
                                                           Wordlist containing words to try to find on server       
    -r|--recursive                                         
                                                           Recurse all found items as if they are directories       
    -u|--username <username>                               
                                                           Username for basic authentication                        
    -p|--password <password>                               
                                                           Password for basic authentication                        
    -t|--request-templates <request_templates>             
                                                           Template defining how to construct URLs to fetch. Default: {BASE_URL}/{WORD}
    -k|--ignore-ssl-errors                                 
                                                           Ignore SSL errors                                        
    -s|--ignored-status-codes <ignore_codes>               
                                                           Ignore any responses having the indicated status codes. Default: 404,400
    -c|--ignored-content-lengths <ignore_content_lengths>  
                                                           Ignore any responses having the indicated content lengths. Default: <none>
    -X|--request-method <request_method>                   
                                                           HTTP method to use when making requests. Possible values are HEAD, POST, GET. Default: HEAD
    -T|--thread-count <thread_count>                       
                                                           Number of threads to use for making requests. Default: 10
    -d|--data <request_data>                               
                                                           Body of the post request. Changes request_method to POST 
    -D|--content-type <content_type>                       
                                                           HTTP content type of the <request_data>. Default: x-www-form-urlencoded
    -H|--headers <headers>                                 
                                                           Headers to add to request. Default <none>                
     <base_url>                                            
                                                           URL to the server to attempt to query
```

This project relies on [cpr](https://github.com/whoshuu/cpr) as a third-party dependency. Other third-party dependencies are submoduled under `include`, so don't forget to `git submodule update --init` after checking out.
