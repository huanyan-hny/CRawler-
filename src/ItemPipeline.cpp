//
// Created by elega on 4/27/17.
//

#include "ItemPipeline.h"



namespace Crawler
{

    void ItemPipeline::handle_items(vector<shared_ptr<Item>>& items )
    {
        for (auto item : items)
        {
            lck.lock();
            process_item(item);
            lck.unlock();
        }

    }
    bool ItemPipeline::simple_write(string path,shared_ptr<Item> item)
    {
        if (path[path.size()-1] != '/')
            path += '/';
        boost::filesystem::path tmp = boost::filesystem::unique_path();
        string filename =  path + tmp.string() + ".txt";
        cout << "writing on " << filename << endl;
        ofstream ofs;
        ofs.open(filename, std::ofstream::out);
        if (ofs.is_open()) {
            ofs << item->to_string();
            ofs.close();
            return true;
        }
        return false;
    }

    bool ItemPipeline::simple_file_write(string path,shared_ptr<Item> item)
    {
        if (path[path.size()-1] != '/')
            path += '/';
        for (pair<string, File> file_item : item->data)
        {
            boost::filesystem::path tmp = boost::filesystem::unique_path();
            string filename = path + tmp.string() + file_item.second.extension;
            ofstream ofs;
            ofs.open(filename, ios::binary | std::ofstream::out);
            if (!ofs.is_open()) return false;
            ofs.write(file_item.second.data.data(), file_item.second.data.size());
            ofs.close();
        }
        return true;
    }
    bool ItemPipeline::simple_file_write(string path,const File& file,string filename) {
        ofstream ofs;
        if (path[path.size()-1] != '/')
            path += '/';
        path = path + string_to_filename(filename) + "." + file.extension;
        ofs.open(path,ios::binary | ios::out);
        if (ofs.is_open()) {
            ofs.write(&(file.data[0]), file.data.size());
            ofs.close();
            return true;
        }
        return false;
    }
    string ItemPipeline::string_to_filename(string input)
    {
        for(int i=0;i<input.size();++i){
            for (int j=0;j<forbiddenChars.size();++j) {
                if (input[i] == forbiddenChars[j])
                    input[i] = '_';
            }
        }
        return input;
    }

}
