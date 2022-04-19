// Initiate datatables in roles, tables, users page
(function() {
    'use strict';
    
    $('#dataTables-example').DataTable({
    	buttons: [
        'copy', 'excel', 'pdf'
    	],
        responsive: true,
        pageLength: 5,
        lengthChange: true,
        searching: false,
        ordering: true,
    });
})();