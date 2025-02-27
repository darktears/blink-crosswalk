# Copyright (C) 2010 Google Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import time
import logging
import re
import urllib
import webapp2

from google.appengine.api import users
from google.appengine.ext.webapp import template
from google.appengine.ext import db

import master_config

from model.jsonresults import JsonResults
from model.testfile import TestFile

PARAM_MASTER = "master"
PARAM_BUILDER = "builder"
PARAM_BUILD_NUMBER = "buildnumber"
PARAM_DIR = "dir"
PARAM_FILE = "file"
PARAM_NAME = "name"
PARAM_BEFORE = "before"
PARAM_NUM_FILES = "numfiles"
PARAM_KEY = "key"
PARAM_TEST_TYPE = "testtype"
PARAM_TEST_LIST_JSON = "testlistjson"
PARAM_CALLBACK = "callback"


def _replace_jsonp_callback(json, callback_name):
    if callback_name and re.search(r"^[A-Za-z0-9_]+$", callback_name):
        if re.search(r"^[A-Za-z0-9_]+[(]", json):
            return re.sub(r"^[A-Za-z0-9_]+[(]", callback_name + "(", json)
        return callback_name + "(" + json + ")"

    return json


class DeleteFile(webapp2.RequestHandler):
    """Delete test file for a given builder and name from datastore."""

    def get(self):
        key = self.request.get(PARAM_KEY)
        # Intentionally don't munge the master from deprecated names here.
        # Assume anyone deleting files wants explicit control.
        master = self.request.get(PARAM_MASTER)
        builder = self.request.get(PARAM_BUILDER)
        test_type = self.request.get(PARAM_TEST_TYPE)
        build_number = self.request.get(PARAM_BUILD_NUMBER, default_value=None)
        name = self.request.get(PARAM_NAME)
        num_files = self.request.get(PARAM_NUM_FILES)
        before = self.request.get(PARAM_BEFORE)

        logging.debug(
            "Deleting File, master: %s, builder: %s, test_type: %s, build_number: %s, name: %s, before: %s, key: %s.",
            master, builder, test_type, build_number, name, before, key)

        limit = int(num_files) if num_files else 1
        num_deleted = TestFile.delete_file(key, master, builder, test_type, build_number, name, before, limit)

        self.response.set_status(200)
        self.response.out.write("Deleted %d files." % num_deleted)


class GetFile(webapp2.RequestHandler):
    """Get file content or list of files for given builder and name."""

    def _get_file_list(self, master, builder, test_type, build_number, name, before, limit, callback_name=None):
        """Get and display a list of files that matches builder and file name.

        Args:
            builder: builder name
            test_type: type of the test
            name: file name
        """

        files = TestFile.get_files(
            master, builder, test_type, build_number, name, before, load_data=False, limit=limit)
        if not files:
            logging.info("File not found, master: %s, builder: %s, test_type: %s, build_number: %s, name: %s.",
                         master, builder, test_type, build_number, name)
            self.response.out.write("File not found")
            return

        template_values = {
            "admin": users.is_current_user_admin(),
            "master": master,
            "builder": builder,
            "test_type": test_type,
            "build_number": build_number,
            "name": name,
            "files": files,
        }
        if callback_name:
            json = template.render("templates/showfilelist.jsonp", template_values)
            self._serve_json(_replace_jsonp_callback(json, callback_name), files[0].date)
            return
        self.response.out.write(template.render("templates/showfilelist.html",
                                                template_values))

    def _get_file_content(self, master, builder, test_type, build_number, name):
        """Return content of the file that matches builder and file name.

        Args:
            builder: builder name
            test_type: type of the test
            build_number: build number, or 'latest'
            name: file name
        """

        files = TestFile.get_files(
            master, builder, test_type, build_number, name, load_data=True, limit=1)
        if not files:
            logging.info("File not found, master %s, builder: %s, test_type: %s, build_number: %s, name: %s.",
                         master, builder, test_type, build_number, name)
            return None, None

        return files[0].data, files[0].date

    def _get_file_content_from_key(self, key):
        file = db.get(key)

        if not file:
            logging.info("File not found, key %s.", key)
            return None

        file.load_data()
        return file.data, file.date

    def _serve_json(self, json, modified_date):
        if json:
            if "If-Modified-Since" in self.request.headers:
                old_date = self.request.headers["If-Modified-Since"]
                if time.strptime(old_date, '%a, %d %b %Y %H:%M:%S %Z') == modified_date.utctimetuple():
                    self.response.set_status(304)
                    return

            # The appengine datetime objects are naive, so they lack a timezone.
            # In practice, appengine seems to use GMT.
            self.response.headers["Last-Modified"] = modified_date.strftime('%a, %d %b %Y %H:%M:%S') + ' GMT'
            self.response.headers["Content-Type"] = "application/json"
            self.response.headers["Access-Control-Allow-Origin"] = "*"
            self.response.out.write(json)
        else:
            self.error(404)

    def get(self):
        key = self.request.get(PARAM_KEY)
        master = self.request.get(PARAM_MASTER)
        builder = self.request.get(PARAM_BUILDER)
        test_type = self.request.get(PARAM_TEST_TYPE)
        build_number = self.request.get(PARAM_BUILD_NUMBER, default_value=None)
        name = self.request.get(PARAM_NAME)
        before = self.request.get(PARAM_BEFORE)
        num_files = self.request.get(PARAM_NUM_FILES)
        test_list_json = self.request.get(PARAM_TEST_LIST_JSON)
        callback_name = self.request.get(PARAM_CALLBACK)

        logging.debug(
            "Getting files, master %s, builder: %s, test_type: %s, build_number: %s, name: %s, before: %s.",
            master, builder, test_type, build_number, name, before)

        if key:
            json, date = self._get_file_content_from_key(key)
        elif num_files or not master or not builder or not test_type or (not build_number and not JsonResults.is_aggregate_file(name)) or not name:
            limit = int(num_files) if num_files else 100
            self._get_file_list(master, builder, test_type, build_number, name, before, limit, callback_name)
            return
        else:
            # FIXME: Stop using the old master name style after all files have been updated.
            master_data = master_config.getMaster(master)
            if not master_data:
                master_data = master_config.getMasterByMasterName(master)
            if not master_data:
                self.error(404)
                return

            json, date = self._get_file_content(master_data['url_name'], builder, test_type, build_number, name)
            if json is None:
                json, date = self._get_file_content(master_data['name'], builder, test_type, build_number, name)

            if json and test_list_json:
                json = JsonResults.get_test_list(builder, json)

        if json:
            json = _replace_jsonp_callback(json, callback_name)

        self._serve_json(json, date)


class Upload(webapp2.RequestHandler):
    """Upload test results file to datastore."""

    def post(self):
        file_params = self.request.POST.getall(PARAM_FILE)
        if not file_params:
            self.response.out.write("FAIL: missing upload file field.")
            return

        builder = self.request.get(PARAM_BUILDER)
        if not builder:
            self.response.out.write("FAIL: missing builder parameter.")
            return

        master_parameter = self.request.get(PARAM_MASTER)

        master_data = master_config.getMasterByMasterName(master_parameter)
        if master_data:
            deprecated_master = master_parameter
            master = master_data['url_name']
        else:
            deprecated_master = None
            master = master_parameter

        test_type = self.request.get(PARAM_TEST_TYPE)

        logging.debug(
            "Processing upload request, master: %s, builder: %s, test_type: %s.",
            master, builder, test_type)

        # There are two possible types of each file_params in the request:
        # one file item or a list of file items.
        # Normalize file_params to a file item list.
        files = []
        logging.debug("test: %s, type:%s", file_params, type(file_params))
        for item in file_params:
            if not isinstance(item, list) and not isinstance(item, tuple):
                item = [item]
            files.extend(item)

        errors = []
        final_status_code = 200
        for file in files:
            file_json = JsonResults._load_json(file.value)
            if file.filename == "incremental_results.json":
                # FIXME: Ferret out and eliminate remaining incremental_results.json producers.
                logging.info("incremental_results.json received from master: %s, builder: %s, test_type: %s.",
                             master, builder, test_type)
                status_string, status_code = JsonResults.update(master, builder, test_type, file_json,
                    deprecated_master=deprecated_master, is_full_results_format=False)
            else:
                try:
                    build_number = int(file_json.get('build_number', 0))
                    status_string, status_code = TestFile.add_file(master, builder, test_type, build_number, file.filename, file.value)
                except (ValueError, TypeError):
                    status_code = 403
                    status_string = 'Could not cast the build_number field in the json to an integer.'

                if status_code == 200:
                    logging.info(status_string)
                else:
                    logging.error(status_string)
                    errors.append(status_string)
                    final_status_code = status_code

            if status_code == 200 and file.filename == "full_results.json":
                status_string, status_code = JsonResults.update(master, builder, test_type, file_json,
                    deprecated_master=deprecated_master, is_full_results_format=True)

            if status_code == 200:
                logging.info(status_string)
            else:
                logging.error(status_string)
                errors.append(status_string)
                final_status_code = status_code

        if errors:
            messages = "FAIL: " + "; ".join(errors)
            self.response.set_status(final_status_code, messages)
            self.response.out.write(messages)
        else:
            self.response.set_status(200)
            self.response.out.write("OK")


class UploadForm(webapp2.RequestHandler):
    """Show a form so user can upload a file."""

    def get(self):
        template_values = {
            "upload_url": "/testfile/upload",
        }
        self.response.out.write(template.render("templates/uploadform.html",
                                                template_values))
