/* *****************************************************************************
   MIT License

   Copyright (c) 2024 I.F.F. dos Santos <ismaellxd@gmail.com>

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the “Software”), to
   deal in the Software without restriction, including without limitation the
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
   sell copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.
***************************************************************************** */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <yaml.h>
#include <tuim.h>
#include "print_version.h"
#include "ptrarray.h"
/* ------------------------------------
   Driver to link objects using the Tuim Developer Kit.
   * Part of tuim project.
   * Last modified: January 6, 2025.
------------------------------------ */

#define STRCMP(lhs, rhs) (strcmp((char*)(lhs), (char*)(rhs)) == 0)
#define strlen_(str) (strlen((char*)(str)) + (size_t)1)
#define strcpy_(dest, src) strcpy((char*)(dest), (char*)(src));
#define ET_EXEC 2U
#define ET_DYN 3U

struct source {
   char *language;
   char *target;
   char **files;
};

struct config {
   char *name;
   unsigned int type;
   char **dependencies;
   struct source **sources;
};

static struct config **parse_yaml(yaml_parser_t *parser);

int main(int argc, char **argv){
   int return_value;
   char *str, *src, *target;
   FILE *stream;
   struct config **configs, **conf;
   yaml_parser_t *parser;

   return_value = EXIT_SUCCESS;
   src = NULL;
   target = tuim_target();
   
   /* Parse arguments and count how many sources must be compiled. */
   for(int i = 1; i < argc; ++i){
      if(argv[i][0] == '-'){
         if(STRCMP(argv[i], "--version")){
            print_version();
            return EXIT_SUCCESS;
         }else if(STRCMP(argv[i], "--target")){
            ++i;
            target = argv[i];
         }else{
            fprintf(stderr, "%s: Unknown option %s.\n", argv[0], argv[i]);
            return EXIT_FAILURE;
         }
      }else{
        src = argv[i];
      }
   }

   if(src == NULL){
      goto end;
   }

   /* --- Parse src/project.yaml --- */

   str = malloc(sizeof("/project.yaml") + strlen_(src));
   if(str == NULL){
      fprintf(stderr, "%s: malloc returned nullptr.\n", argv[0]);
      return_value = EXIT_FAILURE;
      goto end;
   }
   strcpy(str, src);
   strcat(str, "/project.yaml");
   stream = fopen(str, "rb");
   if(stream == NULL){
      goto end;
   }
   free(str);

   parser = malloc(sizeof(yaml_parser_t));
   if(parser == NULL){
      fprintf(stderr, "%s: malloc returned nullptr.\n", argv[0]);
      return_value = EXIT_FAILURE;
      goto end;
   }
   if(!yaml_parser_initialize(parser))
      return EXIT_FAILURE;

   yaml_parser_set_input_file(parser, stream);
   configs = parse_yaml(parser);

   if(configs == NULL){
      fprintf(stderr, "%s: Can't parse %s/project.yaml.\n", argv[0], src);
      return EXIT_FAILURE;
   }

   /* --- Build --- */

   for(struct config **conf = configs; *conf != NULL; ++conf){
      ret = build(args, *conf);
      if(ret != NULL){
         fprintf(stderr, "mk: Can't build %s.\n", ret);
         break;
      }
   }

   end: {
      ptrarray_free(configs, free);
      if(parser != NULL) yaml_parser_delete(parser);
      free(parser);
      if(stream != NULL) fclose(stream);
   }

   return return_value;
}

[[maybe_unused]] static void PrintConfig(const struct config* config){
   struct source **src;
   char **files;

   // Printout the document
   printf("---\n");
   printf("name: %s\n", config->name);
   if(config->type == ET_EXEC)
      printf("object_type: executable\n");
   else if(config->type == ET_DYN)
      printf("object_type: shared\n");
   if(config->dependencies != NULL){
      printf("dependencies:\n");
      for(char **dep = config->dependencies; *dep != NULL; ++dep)
         printf("  - %s\n", *dep);
   }

   if(config->sources == NULL) return;

   printf("sources:\n");

   src = config->sources;
   while(*src != NULL){
      char prefix[] = "  - ";

      if((*src)->language != NULL){
         printf("%slanguage: %s\n", prefix, (*src)->language);
         if(prefix[2] == '-') prefix[2] = ' ';
      }

      if((*src)->target != NULL){
         printf("%starget: %s\n", prefix, (*src)->target);
         if(prefix[2] == '-') prefix[2] = ' ';
      }

      files = (*src)->files;
      if(files != NULL){
         printf("%sfiles:\n", prefix);
         if(prefix[2] == '-') prefix[2] = ' ';
         while(*files){
            printf("      - %s\n", *files);
            ++files;
         }
      }

      ++src;
   }
}

static char ** parse_StrSequence(yaml_document_t *document, yaml_node_t *node){
   yaml_node_item_t *item, *start, *stop;
   yaml_node_t *value;
   char **strings;
   int i;

   strings = malloc(sizeof(char*));
   if(strings == NULL) return strings;
   strings[0] = NULL;

   i = 0;
   start = node->data.sequence.items.start;
   stop = node->data.sequence.items.top;
   for(item = start; item < stop; ++item){
      value = yaml_document_get_node(document, *item);
      strings = ptrarray_strgrow(strings, (char*)value->data.scalar.value);
      if(strings[i] == NULL) return strings;
   }
   return strings;
}

static struct source** parse_sources(
   yaml_document_t *document,
   yaml_node_t *seq
){
   yaml_node_item_t *item, *start, *stop;
   yaml_node_t *node;
   struct source **sources;
   size_t i;

   sources = malloc(sizeof(struct source*));
   if(sources == NULL) return sources;
   sources[0] = NULL;

   i = (size_t)0;
   start = seq->data.sequence.items.start;
   stop = seq->data.sequence.items.top;
   for(item = start; item < stop; ++item){
      yaml_node_pair_t *pair_, *start_, *stop_;
      yaml_node_t *key_, *value_;

      node = yaml_document_get_node(document, *item);

      if(node->type != YAML_MAPPING_NODE){
         /*TODO: handle error */
         ptrarray_free(sources, free);
         return NULL;
      }

      sources = ptrarray_grow(sources, NULL, sizeof(struct source));
      if(sources[i] == NULL) return sources;
      sources[i]->language = NULL;
      sources[i]->target = NULL;
      sources[i]->files = NULL;

      start_ = node->data.mapping.pairs.start;
      stop_ = node->data.mapping.pairs.top;
      for(pair_ = start_; pair_ < stop_; ++pair_){
         key_ = yaml_document_get_node(document, pair_->key);
         value_ = yaml_document_get_node(document, pair_->value);

         if(value_->type == YAML_SCALAR_NODE){
            char **str;
            if(STRCMP(key_->data.scalar.value, "target"))
               str = &(sources[i]->target);
            else if(STRCMP(key_->data.scalar.value, "language"))
               str = &(sources[i]->language);
            *str = malloc(strlen_(value_->data.scalar.value));
            strcpy_(*str, value_->data.scalar.value);
         }else if(value_->type == YAML_SEQUENCE_NODE){
            if(STRCMP(key_->data.scalar.value, "files"))
               sources[i]->files = parse_StrSequence(document, value_);
         }else if(value_->type == YAML_MAPPING_NODE){
            // Nothing to do
         }
      }
      ++i;
   }
   return sources;
}

static void parse_document(yaml_document_t *document, struct config *config){
   yaml_node_pair_t *pair, *start, *stop;
   yaml_node_t *node, *key, *value;

   // Is expected that the first node is the main mapping node
   node = yaml_document_get_node(document, 1);
   if(node->type != YAML_MAPPING_NODE) return;

   start = node->data.mapping.pairs.start;
   stop = node->data.mapping.pairs.top;
   for(pair = start; pair < stop; ++pair){
      key = yaml_document_get_node(document, pair->key);
      value = yaml_document_get_node(document, pair->value);

      if(value->type == YAML_SCALAR_NODE){
         if(STRCMP(key->data.scalar.value, "name")){
            config->name = malloc(strlen_(value->data.scalar.value));
            strcpy_(config->name, value->data.scalar.value);
         }else if(STRCMP(key->data.scalar.value, "object_type")){
            if(STRCMP(value->data.scalar.value, "executable"))
               config->type = ET_EXEC;
            else if(STRCMP(value->data.scalar.value, "shared"))
               config->type = ET_DYN;
         }
      }else if(value->type == YAML_SEQUENCE_NODE){
         if(STRCMP(key->data.scalar.value, "dependencies"))
            config->dependencies = parse_StrSequence(document, value);
         else if(STRCMP(key->data.scalar.value, "sources"))
            config->sources = parse_sources(document, value);
      }else if(value->type == YAML_MAPPING_NODE){
         // Nothing to do
      }
   }
}

static struct config **parse_yaml(yaml_parser_t *parser){
   yaml_document_t *document;
   struct config **configs;
   int i;

   configs = malloc(sizeof(struct config*));
   document = malloc(sizeof(yaml_document_t));

   i = (size_t)0;
   configs[0] = NULL;
   while((
      yaml_parser_load(parser, document),
      yaml_document_get_root_node(document) != NULL
   )){
      configs = ptrarray_grow(configs, NULL, sizeof(struct config));
      if(configs[i] == NULL) return NULL;

      configs[i]->name = NULL;
      configs[i]->dependencies = NULL;
      configs[i]->sources = NULL;

      parse_document(document, configs[i]);
      yaml_document_delete(document);

      //PrintConfig(configs[i]);

      ++i;
   }
   free(document);
   return configs;
}

static char* SrcName(const char *SourceName, const char *prefix){
   char *buf;
   size_t len;

   len = strlen(SourceName) + strlen(prefix);
   buf = malloc(len + (size_t)2);
   if(buf != NULL){
      strcpy(buf, prefix);
      strcat(buf, "/");
      strcat(buf, SourceName);
   }
   return buf;
}

static char* ObjName(const char *SourceName){
   char *buf;
   size_t len;

   len = strlen(SourceName) + (size_t)4;
   buf = malloc(len + (size_t)1);
   if(buf != NULL){
      strcpy(buf, "tmp/");
      strcat(buf, SourceName);
      while(buf[len] != '.') --len;
      ++len, buf[len] = 'o';
      ++len, buf[len] = '\0';
   }
   return buf;
}

#define STATIC static

#define MAIN ld
#include "ld.c"
#undef MAIN

#define MAIN as
#include "as.c"
#undef MAIN

#define MAIN cc
#include "cc.c"
#undef MAIN

static void* build(struct config *conf, const char *src, ){
   tdk_ld_args_t *ld_args;
   char *ret;
   struct stat sb;

   ret = NULL;

   /* --- Set up directories --- */
   /* FIXME: Replace POSIX API by a more convenient library.
      POSIX API do not use a prefix for names, this may be not a good pratice.
      Moreover, some function from the POSIX API are declared on standard
      headers and are missing on non POSIX systems, like Tuim environments. */
   if(stat("tmp", &sb) != 0 && !S_ISDIR(sb.st_mode)){
      if(mkdir("tmp", 0777) == -1){
         perror("mkdir");
         goto end;
      }
   }
   if(conf->type == ET_EXEC){
      if(stat("bin", &sb) != 0 && !S_ISDIR(sb.st_mode)){
         if(mkdir("bin", 0777) == -1){
            perror("mkdir");
            goto end;
         }
      }
   }else if(conf->type == ET_DYN){
      if(stat("lib", &sb) != 0 && !S_ISDIR(sb.st_mode)){
         if(mkdir("lib", 0777) == -1){
            perror("mkdir");
            goto end;
         }
      }
   }

   /* --- Parse conf --- */

   if(conf->name == NULL) return NULL;
   if(conf->sources == NULL) return NULL;

   ld_args = tdk_ld_args_defaults();
   if(conf->type == ET_EXEC){
      char *exec;
      size_t len;
      len = strlen(conf->name) + (size_t)9;
      exec = malloc(len);
      strcpy(exec, "bin/");
      strcat(exec, conf->name);
      strcat(exec, ".elf");
      tdk_ld_args_SetOutput(ld_args, exec);
      free(exec);
   }else if(conf->type == ET_DYN){
      char *exec;
      size_t len;

      tdk_ld_args_EnableShared(ld_args);

      len = strlen(conf->name) + (size_t)11;
      exec = malloc(len);
      strcpy(exec, "lib/lib");
      strcat(exec, conf->name);
      strcat(exec, ".so");
      tdk_ld_args_SetOutput(ld_args, exec);
      free(exec);
   }

   for(struct source **src = conf->sources; *src != NULL; ++src){
      if((*src)->files == NULL) goto end;
      if((*src)->target != NULL)
         if(!STRCMP(args->target, (*src)->target)) goto end;

      if((*src)->language != NULL)
      /* --- compile --- */ {
         if(STRCMP((*src)->language, "C")){
            for(char **fil = (*src)->files; *fil != NULL; ++fil){
               tdk_cc_args_t *cc_args;
               char *obj, *src;

               src = SrcName(*fil, args->input);
               obj = ObjName(*fil);

               cc_args = tdk_cc_args_defaults();
               tdk_cc_args_SetTarget(cc_args, args->target);

               tdk_cc_args_AddInput(cc_args, src);
               tdk_cc_args_SetOutput(cc_args, obj);
               tdk_ld_args_AddInput(ld_args, obj);

               if(tdk_cc(cc_args) == EXIT_FAILURE){
                  ret = src;
                  free(obj);
                  tdk_cc_args_free(cc_args);
                  goto end;
               }

               free(src);
               free(obj);
               tdk_cc_args_free(cc_args);
            }
         }
      }else
      /* --- assemble --- */ {
         for(char **fil = (*src)->files; *fil != NULL; ++fil){
            tdk_as_args_t *as_args;
            char *obj, *src;

            src = SrcName(*fil, args->input);
            obj = ObjName(*fil);

            as_args = tdk_as_args_defaults();
            tdk_as_args_SetTarget(as_args, args->target);

            tdk_as_args_AddInput(as_args, src);
            tdk_as_args_SetOutput(as_args, obj);
            tdk_ld_args_AddInput(ld_args, obj);

            if(tdk_as(as_args) == EXIT_FAILURE){
               ret = src;
               free(obj);
               tdk_as_args_free(as_args);
               goto end;
            }

            free(src);
            free(obj);
            tdk_as_args_free(as_args);
         }
      }
   }

   /* --- link --- */

   if(conf->dependencies != NULL){
      for(char **dep = conf->dependencies; *dep != NULL; ++dep){
         char *lib;
         size_t len;
         len = strlen(*dep) - (size_t)6;
         lib = malloc(len + (size_t)1);
         memcpy(lib, *dep + 3, len);
         lib[len] = '\0';
         tdk_ld_args_LinkLibrary(ld_args, lib);
         free(lib);
      }
   }

   tdk_ld(ld_args);

   end:
   tdk_ld_args_free(ld_args);

   return ret;
}
