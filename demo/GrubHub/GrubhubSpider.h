#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Spider.h"
#include "../../include/CRawlerPlusPlus/Json.h"

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
                          callback(&GrubhubSpider::parse_restaurant),Auth_type::OAUTH2,Authentication("Bearer","582d1eef-6898-4576-a37f-f106cf110e1a")).
                    put_string("auth","582d1eef-6898-4576-a37f-f106cf110e1a"));
        }

        void parse_restaurant(shared_ptr<Task> task, shared_ptr<Response> res,
                              function<void(Task&)> add_task, function<void(Item&)> produce_item){
            vector<string> ids {"240271", "66328", "287583", "66367", "309616", "296705"};
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

            Json j = Json::parse(res->asio_response);
            Item item;
            vector<Json> result = j.find_by_key("menu_category_list");
            vector<Json> list = result[0].find_by_key("menu_item_list");
            for (int j = 0; j < list.size();j++) {
                Json menu = list[j];
                for (int i=0;i<menu.size();i++) {
                    item.type = "menu";
                    item.name = task->bundle["id"];
                    item["name"] =  menu[i]["name"].toString();
                    item["price"] = menu[i]["price"]["amount"].toString();
                    produce_item(item);
                }
            }


        }

    };
}