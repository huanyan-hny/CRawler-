#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Spider.h"

using namespace std;

namespace Crawler
{
    using namespace Parser;
    class GrubhubSpider: public Spider
    {
    public:
        void initial_tasks(function<void(Task&)> add_task)
        {
            add_task(Task("https://api-gtm.grubhub.com/restaurants/search?orderMethod=delivery&locationMode=DELIVERY&facetSet=umamiV2&pageSize=20&hideHateos=true&location=POINT(-73.96379853%2040.79992675)&facet=open_now%3Atrue&variationId=dishReorderConversionScore&sortSetId=umamiV2&sponsoredSize=3&countOmittingTimes=true",
                          callback(&GrubhubSpider::parse_restaurant),Auth_type::OAUTH2,Authentication("Bearer","97cf1352-9701-4700-a80c-df7915a2f9ab")).
                    put_string("auth","97cf1352-9701-4700-a80c-df7915a2f9ab"));
        }

        void parse_restaurant(shared_ptr<Task> task, shared_ptr<Response> res,
                              function<void(Task&)> add_task, function<void(Item&)> produce_item){
            vector<string> ids {"240271", "66328", "287583", "66367", "309616", "296705", "309617"};
            for (auto & id: ids) {
                cout << id << endl;
                add_task(Task("https://api-gtm.grubhub.com/restaurants/"+id,
                              callback(&GrubhubSpider::parse_menu),Request_method::POST,
                              Auth_type::OAUTH2,Authentication("Bearer",task->bundle["auth"]),
                              Form ({{"hideUnavailableMenuItems","true"}})).put_string("id",id));
            }
        }

        void parse_menu(shared_ptr<Task> task, shared_ptr<Response> res,
                         function<void(Task&)> add_task, function<void(Item&)> produce_item)
        {
            cout << "Done! " << task->bundle["id"] << " " << res->asio_response.substr(0,10) << endl;
            Item i;
            i.type = "menu";
            i.name = task->bundle["id"];
            i["json"] = res->asio_response;
            i["movie_url"] = task->get_url();
            produce_item(i);
        }

    };
}