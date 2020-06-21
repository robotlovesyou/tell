#ifndef TELL_GEN_HTMLTEMPLATE_H_
#define TELL_GEN_HTMLTEMPLATE_H_

/**
 * inja template for HTML Documentation
 */
static const char *html_template = R"TEMPLATE(<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Tell Generated Documentation</title>
    <!-- CSS only -->
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css"
        integrity="sha384-9aIt2nRpC12Uk9gS9baDl411NQApFmC26EwAOH8WgZl5MYYxFfc+NcPb1dKGj7Sk" crossorigin="anonymous">

</head>

<body class="bg-light">
    <div class="container">
        <div class="jumbotron jumbotron-fluid">
            <div class="container">
                <h1 class="display-4">Tell Generated Documentation</h1>
            </div>
        </div>
    </div>
    <div class="container">
        <ul class="nav nav-pills">
            <li class="nav-item dropdown">
                <a class="nav-link dropdown-toggle" data-toggle="dropdown" href="#" role="button"
                    aria-haspopup="true" aria-expanded="false">Messages</a>
                <div class="dropdown-menu">
                    {%for message in message_declarations%}
                    <a class="dropdown-item" href="#MESSAGE_{{message.name}}">{{message.name}}</a>
                    {%endfor%}
                </div>
            </li>
            <li class="nav-item">
                <a class="nav-link dropdown-toggle" data-toggle="dropdown" href="#" role="button"
                    aria-haspopup="true" aria-expanded="false">Services</a>
                <div class="dropdown-menu">
                    {%for service in service_declarations%}
                    <a class="dropdown-item" href="#SERVICE_{{service.name}}">{{service.name}}</a>
                    {%endfor%}
                </div>
            </li>
        </ul>
    </div>
    <div class="container">
        <h1 class="display-4">Messages</h1>
        {%for message in message_declarations%}
        <div class="container">
            <h4 id="MESSAGE_{{message.name}}">{{message.name}}</h4>
            <p>{{message.doc_comment}}</p>
            <table class="table table-striped">
                <thead>
                    <tr>
                        <th scope="col">Field Name</th>
                        <th scope="col">Type</th>
                        <th scope="col">Documenation</th>
                    </tr>
                </thead>
                <tbody>
                    {%for field in message.fields%}
                    <tr>
                        <td>{{field.name}}</td>
                        <td>{{map_type(field.type_def)}}</td>
                        <td>{{field.doc_comment}}</td>
                    </tr>
                    {%endfor%}
                </tbody>
            </table>
            <hr />
        </div>
        {%endfor%}
    </div>
    <div class="container">
        <h1 class="display-4">Services</h1>
        {%for service in service_declarations%}
        <div class="container">
            <h4 id="SERVICE_{{service.name}}">{{service.name}}</h4>
            <p>{{service.doc_comment}}</p>
            <table class="table table-striped">
                <thead>
                    <tr>
                        <th scope="col">Call Name</th>
                        <th scope="col">Argument Type</th>
                        <th scope="col">Return Type</th>
                        <th scope="col">Documentation</th>
                    </tr>
                </thead>
                <tbody>
                    {%for call in service.calls%}
                    <tr>
                        <td>{{call.name}}</td>
                        <td><a href="#MESSAGE_{{call.argument}}">{{call.argument}}</a></td>
                        <td><a href="#MESSAGE_{{call.returns}}">{{call.returns}}</a></td>
                        <td>{{call.doc_comment}}</td>
                    </tr>
                    {%endfor%}
                </tbody>
            </table>
            <hr />
        </div>
        {%endfor%}
    </div>
</body>
<script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"
    integrity="sha384-DfXdz2htPH0lsSSs5nCTpuj/zy4C+OGpamoFVy38MVBnE+IbbVYUew+OrCXaRkfj"
    crossorigin="anonymous"></script>
<script src="https://cdn.jsdelivr.net/npm/popper.js@1.16.0/dist/umd/popper.min.js"
    integrity="sha384-Q6E9RHvbIyZFJoft+2mJbHaEWldlvI9IOYy5n3zV9zzTtmI3UksdQRVvoxMfooAo"
    crossorigin="anonymous"></script>
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/js/bootstrap.min.js"
    integrity="sha384-OgVRvuATP1z7JjHLkuOU7Xw704+h835Lr+6QL9UvYjZE3Ipu6Tp75j7Bh/kR0JKI"
    crossorigin="anonymous"></script>

</html>)TEMPLATE";
#endif //TELL_GEN_HTMLTEMPLATE_H_
